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
import java.io.IOException;
import java.util.Arrays;
import java.util.Iterator;
import java.util.List;

public class DiskDump {  
    static String driveId = "DRIVE";
    static String traceFilePath = null;
    static String outputPath = ".";
    
    /**
     * @param args the command line arguments
     * @throws java.io.IOException
     */
    public static void main(String[] args) throws IOException {         
        if(args.length == 0) {
            PrintHelp();
            return;
        }
        
        List arguments = Arrays.asList(args);
        
        Iterator<String> argumentsIterator = arguments.iterator();
        
        while(argumentsIterator.hasNext())
        {
            String argument = argumentsIterator.next();
            
            switch (argument) {
                case "-i":
                case "-input":
                    if(argumentsIterator.hasNext() == false)
                    {
                        System.out.println("Missing parameter for the " + argument + "option.  See -? for help.");
                        return;
                    }
                    
                    traceFilePath = argumentsIterator.next();
                    
                    break;
                case "-o":
                case "-output":
                    if(argumentsIterator.hasNext() == false)
                    {
                        System.out.println("Missing parameter for the " + argument + "option.  See -? for help.");
                        return;
                    }
                    
                    outputPath = argumentsIterator.next();
                    
                    break;
                case "-d":
                case "-drivename":
                    if(argumentsIterator.hasNext() == false)
                    {
                        System.out.println("Missing parameter for the " + argument + "option.  See -? for help.");
                        return;
                    }
                    
                    driveId = argumentsIterator.next();
                    
                    if(driveId.charAt(0) == '-'){
                        System.out.println("Missing parameter for the " + argument + "option.  See -? for help.");
                        return;
                    }
                    
                    if(Character.isDigit(driveId.charAt(0)))
                    {
                        System.out.println("Invalid DriveId: " + argument + ".  DriveId must be a valid C name.  It can't begin with a number.");
                        return;
                    }
                    
                    break;
                    
                case "-?":
                case "-help":
                    PrintHelp();
                    
                default:
                    System.out.println("Unknown command: " + argument + ".  See -? for help.");
                    return;
            }
        }

        
        if(traceFilePath == null) {
            System.out.println("A trace file must be specified using the -i, -input option.");
        } else {
            FatFsEmulatedDiskGenerator generator = new FatFsEmulatedDiskGenerator(outputPath, driveId);
            TraceParser trace = new BeagleCollapsedClassViewTraceParcer(generator);
            trace.parse(traceFilePath);
        }
    }
    
    private static void PrintHelp(){
        final String[] helpMessage = {
            "DiskDump tool takes in an exported USB trace from the Total Phase",
            "Data Center(tm) and converts it into a .c file that can be used with",
            "emulated drive test platform for the FILEIO library.",
            "",
            "Options:",
            "",
            "    -?, -help: this help menu",
            "",
            "    -i, -input: input trace file location.  This file should be a",
            "    .csv file obtained by taking a USB trace from Data Center(tm) and",
            "    exporting it.  All options should be unchecks on the export such",
            "    that all data is exported.",
            "",
            "    -o, -output: the folder in which to generate the output" ,
            "",
            "    -d, -drivename: the name given to the drive.  This will be used to",
            "    name the output files as well as the functions and identifiers",
            "    associated with this drive."
        };
        
        for(String line: helpMessage)
        {
            System.out.println(line);
        }
    }
}
