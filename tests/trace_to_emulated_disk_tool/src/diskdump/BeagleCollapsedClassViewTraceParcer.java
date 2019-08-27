/*******************************************************************************
Copyright 2018 Microchip Technology Inc. (www.microchip.com)

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.

To request to license the code under the MLA license (www.microchip.com/mla_license), 
please contact mla_licensing@microchip.com
*******************************************************************************/
package diskdump;

import java.io.BufferedReader;
import java.io.FileNotFoundException;
import java.io.FileReader;
import java.io.IOException;
import java.util.Arrays;
import java.util.HashSet;
import java.util.Set;
import java.util.logging.Level;
import java.util.logging.Logger;

public class BeagleCollapsedClassViewTraceParcer implements TraceParser{
    private final EmulatedDiskGenerator generator;
    private final Set<Integer> sectors;
    
    public BeagleCollapsedClassViewTraceParcer(EmulatedDiskGenerator generator) throws FileNotFoundException
    {
        this.generator = generator;
        sectors = new HashSet<>();
    }
    
    @Override
    public void parse(String traceFile)
    {
        sectors.clear();
        
        try {
            BufferedReader reader = new BufferedReader(new FileReader(traceFile));
            byte[] sectorData;
            String traceLine;

            while ((traceLine = reader.readLine()) != null) {                
                if(isReadOperation(traceLine) == true)
                {
                    ReadLine line = new ReadLine(traceLine);
                    System.out.print("Read[0] found: LBA = ");
                    System.out.print(line.getStartingLBA());
                    System.out.print(" Length = ");
                    System.out.print(line.getSectorCount());
                    System.out.println();
                    
                    for(Integer i = 0; i < line.getSectorCount(); i++)
                    {
                        Integer lba = line.getStartingLBA()+i;
                        if(sectors.add(lba) == true)
                        {
                            sectorData = line.getSectorData(lba);
                            if(MathHelper.isZero(sectorData) == false)
                            {
                                generator.addSector(lba, sectorData);
                            }   
                        }
                    }
                    

                }
            }

            reader.close();
            generator.complete();
        } catch (IOException ex) {
            Logger.getLogger(BeagleCollapsedClassViewTraceParcer.class.getName()).log(Level.SEVERE, null, ex);
        }
    }
    
    private class ReadLine
    {
        private Integer lba;
        private Integer sectorCount;
        private final Integer sectorSize = 512;
        private byte[] data;
        
        public ReadLine(String line)
        {
           lba = getBlockLBA(line);
           sectorCount = getSectorCount(line);
           data = getBlockData(line);
        }
        
        public Integer getStartingLBA()
        {
            return lba;
        }
        
        public Integer getSectorCount()
        {
            return sectorCount;
        }        
        
        public byte[] getSectorData(Integer requested_lba)
        {                        
            return Arrays.copyOfRange(data, (requested_lba-lba)*sectorSize, (requested_lba-lba)*sectorSize+sectorSize);
        }
        
        private Integer getBlockLBA(String TraceLine)
        {
            int StartIndex;
            int EndIndex;

            StartIndex = TraceLine.indexOf("LBA = ") + "LBA = ".length();
            EndIndex = TraceLine.indexOf(" ", StartIndex);

            return Integer.parseInt(TraceLine.substring(StartIndex, EndIndex));
        }

        private Integer getSectorCount(String TraceLine)
        {
            int StartIndex;
            int EndIndex;

            StartIndex = TraceLine.indexOf("Length = ") + "Length = ".length();
            EndIndex = TraceLine.indexOf(" ", StartIndex);

            return Integer.parseInt(TraceLine.substring(StartIndex, EndIndex));
        }
        
        private byte[] getBlockData(String TraceLine)
        {
            int StartIndex;
            int EndIndex;
            String stringData;
            int string_offset;
            byte[] sector_data;

            StartIndex = TraceLine.indexOf("Read [0],") + "Read [0],".length();
            EndIndex = TraceLine.indexOf(",LBA =", StartIndex);
            
            stringData = TraceLine.substring(StartIndex, EndIndex);
            
            sector_data = new byte[sectorSize * sectorCount];
            string_offset = 0;
            
            for(int i = 0; i < sectorSize * sectorCount; i++)
            {
                sector_data[i] = (byte)((Character.digit(stringData.charAt(string_offset), 16) << 4) + Character.digit(stringData.charAt(string_offset+1), 16));
                string_offset += 3;
            }
            
            return sector_data;
        }
    }
    
    private boolean isReadOperation(String TraceLine)
    {
        return TraceLine.contains("Read [0]");
    }
    

}
