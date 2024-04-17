using System;
using System.Collections.Generic;
using System.IO.Enumeration;

public struct input
{
    public int n;
    public char[][] B;
    public int x, y;

}

public class Tilt_Game
{
    
    public input read_test_file(string filename)
    {
        //Read file
        string directoryPath = "Sample Tests";
        string filePath = Path.Combine(directoryPath, filename);
        FileStream file = new FileStream(filePath, FileMode.Open, FileAccess.Read);
        StreamReader sr = new StreamReader(file);

        input values;

        //Read board dimention (n)
        string line = sr.ReadLine();
        values.n = int.Parse(line);

        //Read board (B)
        values.B = new char[values.n][];
        for(int i = 0; i <  values.n; i++)
        {
            line = sr.ReadLine();
            values.B[i] = line.Where(c => !char.IsWhiteSpace(c)).ToArray();
        }

        //Read target dimentions (x, y)
        line = sr.ReadLine();
        string[] target = line.Split(", ");
        values.x = int.Parse(target[0]);
        values.y = int.Parse(target[1]);

        return values;
    }
}

class Program
{
    

    
    
    static void Main(string[] args)
    {
        Tilt_Game game = new Tilt_Game();
        input values = game.read_test_file("Case1.txt");
        
    }


    
}