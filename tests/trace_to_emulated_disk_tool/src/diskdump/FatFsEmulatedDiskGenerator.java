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

import java.io.FileWriter;
import java.io.IOException;
import java.io.PrintWriter;

public class FatFsEmulatedDiskGenerator extends EmulatedDiskGenerator{
    private final PrintWriter source; 
    private String initializeFunction;
    private final String driveId;
    private final String license =  
        "/*******************************************************************************\n" +
        "Copyright 2018 Microchip Technology Inc. (www.microchip.com)\n" +
        "\n" +
        "Licensed under the Apache License, Version 2.0 (the \"License\");\n" +
        "you may not use this file except in compliance with the License.\n" +
        "You may obtain a copy of the License at\n" +
        "\n" +
        "    http://www.apache.org/licenses/LICENSE-2.0\n" +
        "\n" +
        "Unless required by applicable law or agreed to in writing, software\n" +
        "distributed under the License is distributed on an \"AS IS\" BASIS,\n" +
        "WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.\n" +
        "See the License for the specific language governing permissions and\n" +
        "limitations under the License.\n" +
        "\n" +
        "To request to license the code under the MLA license (www.microchip.com/mla_license), \n" +
        "please contact mla_licensing@microchip.com\n" +
        "*******************************************************************************/";
    
    public FatFsEmulatedDiskGenerator(String path, String driveId) throws IOException
    {
        source = new PrintWriter(new FileWriter(path + "/" + driveId + ".c"));
       
        this.driveId = driveId;
        initializeFunction = new String();
        
        source.println(license);
        source.println("#include <stdlib.h>");
        source.println("#include <stdint.h>");
        source.println("#include <stddef.h>");
        source.println("#include <stdio.h>");
        source.println("#include <string.h>");
        source.println("");
        source.println("#include \"emulated_disk.h\"");
        source.println("");
        source.println("#ifdef SECTOR_SIZE");
        source.println("#error \"Sector size already defined.\"");
        source.println("#endif");
        source.println("");
        source.println("#define SECTOR_SIZE 512");
        source.println("");
    }
            
    @Override
    public void addSector(long lba, byte[] data)
    {     
        source.print("static const uint8_t sector" + lba + "[] = { ");

        for(byte b: data)
        {
            source.printf("0x%02X, ", b);
        }

        source.println("};");
        
        initializeFunction += "    EmulatedDiskSectorWrite(NULL, " + lba + ", (uint8_t*)sector" + lba + ", " + Boolean.toString(lba==0) + ");" + System.getProperty("line.separator");        
    }
    
    @Override
    public void complete()
    {        
        source.println("");
        source.println("static struct EMULATED_DISK disk = {0, NULL};");
        source.println("");
        source.println("void " + driveId + "_initialize(void)");
        source.println("{    ");
        source.println("    EmulatedDiskCreate(&disk, SECTOR_SIZE);");
        source.println("    ");
        source.println(initializeFunction);
        source.println("}");
        source.println("");
        source.println("void " + driveId + "_print(void)");
        source.println("{");
        source.println("    EmulatedDiskPrint(&disk);");
        source.println("}");
        source.println("");
        source.println("static const char id[] = \"" + driveId + "\";");
        source.println("const char* " + driveId + "_id(void)");
        source.println("{");
        source.println("    return id;");
        source.println("}");
        source.println("");
        source.println("struct EMULATED_DRIVE " + driveId + " = { &" + driveId + "_initialize, &" + driveId + "_print, &" + driveId + "_id};");
        source.println("");       
        source.close();
    }
}
