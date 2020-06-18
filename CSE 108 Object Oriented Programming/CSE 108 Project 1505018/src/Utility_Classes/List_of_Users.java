
package Utility_Classes;


import java.io.Serializable;
import java.util.Vector;

public class List_of_Users implements Serializable
{
    
    public Vector<Client_ID> v;
    public long SZ;
    public List_of_Users()
    {
        v = new Vector<>();
        SZ = -1;
    }
    @Override
    public String toString()
    {
        return v.toString();
        
    }
}