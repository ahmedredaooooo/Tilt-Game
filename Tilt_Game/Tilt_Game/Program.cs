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
        string directoryPath = "Sample Tests";
        string filePath = Path.Combine(directoryPath, filename);

        FileStream file = new FileStream(filePath, FileMode.Open, FileAccess.Read);

        StreamReader sr = new StreamReader(file);
        string line = sr.ReadLine();
        input values;
        values.n = int.Parse(line);
        values.B = new char[values.n][];
        for(int i = 0; i <  values.n; i++)
        {
            line = sr.ReadLine();
            values.B[i] = line.Where(c => !char.IsWhiteSpace(c)).ToArray();
        }
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
        Console.WriteLine(values.n);
        Console.WriteLine(values.x); Console.WriteLine(values.y);
        

    }


    
}