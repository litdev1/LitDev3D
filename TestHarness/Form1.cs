using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using Microsoft.SmallBasic.Library;
using FileIO = System.IO.File;

namespace LitDev3D
{
    public partial class Form1 : Form
    {
        public Form1()
        {
            InitializeComponent();
        }

        private void button1_Click(object sender, EventArgs e)
        {
            TextWindow.Left = 0;
            LD3D.Setup("False", "LightBlue");

            Primitive cube = LD3D.AddCube();
            LD3D.SetTexture(cube, "C:\\Users\\Steve\\Documents\\irrlicht\\irrlicht-1.7.3\\media\\t351sml.jpg");

            Primitive sphere = LD3D.AddSphere();
            LD3D.SetTexture(sphere, "C:\\Users\\Steve\\Documents\\irrlicht\\irrlicht-1.7.3\\media\\wall.bmp");
            LD3D.SetPosition(sphere, 0, 0, 30);

            //Primitive ninja = LD3D.AddAnimatedMesh("ninja.b3d", 0, 13, 10);
            //LD3D.SetTexture(ninja, "sydney.bmp");
            Primitive ninja = LD3D.AddNinja("Blue");
            LD3D.SetPosition(ninja, 0, 0, 30);
            LD3D.SetRotation(ninja, 0, 90, 0);
            LD3D.SetScale(ninja, 5, 5, 5);
            LD3D.SetGlow(ninja, "Blue", 100);

            //LD3D.AddTerrain("terrain-heightmap.bmp", "terrain-texture.jpg", "detailmap3.jpg", 0);
            //LD3D.AddMountainTerrain(10);

            LD3D.AddDungeonMesh(10);

            LD3D.SetGravity(ninja, 10);
            LD3D.SetPosition(ninja, 0, -235, 0);
            LD3D.AnimateCircle(ninja, 50, 90);
            LD3D.GetType(ninja);
            //LD3D.AnimateRelative(ninja, 1000, 0, 0, 10000);

            LD3D.Start();
            TextWindow.Read();
        }

        private void button2_Click(object sender, EventArgs e)
        {
            string environmentVariable = Environment.GetEnvironmentVariable("PROCESSOR_ARCHITECTURE");
            if (environmentVariable.Contains("64"))
            {
                FileIO.Copy(Application.StartupPath + "\\LitDev3D.dll", "C:\\Program Files (x86)\\Microsoft\\Small Basic\\lib\\LitDev3D.dll", true);
                FileIO.Copy(Application.StartupPath + "\\LitDev3D.xml", "C:\\Program Files (x86)\\Microsoft\\Small Basic\\lib\\LitDev3D.xml", true);
                FileIO.Copy(Application.StartupPath + "\\LitDev3DEngine.dll", "C:\\Program Files (x86)\\Microsoft\\Small Basic\\lib\\LitDev3DEngine.dll", true);
            }
            else
            {
                FileIO.Copy(Application.StartupPath + "\\LitDev3D.dll", "C:\\Program Files\\Microsoft\\Small Basic\\lib\\LitDev3D.dll", true);
                FileIO.Copy(Application.StartupPath + "\\LitDev3D.xml", "C:\\Program Files\\Microsoft\\Small Basic\\lib\\LitDev3D.xml", true);
                FileIO.Copy(Application.StartupPath + "\\LitDev3DEngine.dll", "C:\\Program Files\\Microsoft\\Small Basic\\lib\\LitDev3DEngine.dll", true);
            }
        }

        private void button3_Click(object sender, EventArgs e)
        {
            Close();
        }
    }
}
