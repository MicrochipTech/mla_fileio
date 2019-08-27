/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package diskdump;

public class MathHelper {
    static public boolean isZero(final byte[] data) {
        for (byte b : data) 
        {
            if(b != 0)
            {
                return false;
            }
        }
        return true;
    }
}
