/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package diskdump;

abstract class EmulatedDiskGenerator {
    public EmulatedDiskGenerator(){
        
    }
    abstract public void addSector(long lba, byte[] data);
    abstract public void complete();
}
