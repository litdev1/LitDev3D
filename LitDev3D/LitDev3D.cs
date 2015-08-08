using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using Microsoft.SmallBasic.Library;
using System.Runtime.InteropServices;
using LitDev3D.Properties;
using System.Drawing;
using System.IO;
using System.Threading;
using System.Windows.Input;
using FileIO = System.IO.File;

namespace LitDev3D
{
    public class Node
    {
        public int id;
        public string name;
        public string type;
        public Node(int _id)
        {
            id = _id;
            name = "Node" + id.ToString();
            type = "";
        }
        public Node(int _id, string _name)
        {
            id = _id;
            name = _name + id.ToString();
            type = "";
        }
    }

    /// <summary>
    /// A 3D engine for SmallBasic using the irrlicht engine (http://irrlicht.sourceforge.net/).
    ///
    /// The coordinate system is X (left to right) Y (down to up) and Z (front to back).
    /// Move with the arrow keys or WSAD and use Escape to end.
    /// </summary>
    [SmallBasicType]
    public class LD3D
    {
        private static EngineCLR.MessageDelegate messageDelegate = new EngineCLR.MessageDelegate(Message);
        private static EngineCLR Engine = new EngineCLR(messageDelegate);
        private static SmallBasicCallback TimerDelegate = null;
        private static SmallBasicCallback CollisionDelegate = null;
        private static SmallBasicCallback ClickDelegate = null;
        private static String collisionObject1, collisionObject2, clickObject;
        private static bool workingTimer = false;
        private static bool workingCollision = false;
        private static bool workingClick = false;
        private static bool running = false;
        private static bool consoleMessages = false;
        private static KeyConverter kc = new KeyConverter();

        private static List<Node> Nodes = new List<Node>();
        private static int getNodeID()
        {
            int num = 1;
            foreach (Node current in Nodes)
            {
                if (num <= current.id)
                {
                    num = current.id + 1;
                }
            }
            return num;
        }
        private static Node getNode(string name)
        {
            foreach (Node current in Nodes)
            {
                if (current.name == name)
                {
                    return current;
                }
            }
            if (consoleMessages) Console.WriteLine("Error : Object not found " + name);
            return null;
        }

        private static string ninja = string.Empty;
        private static string nskinrd = string.Empty;
        private static string nskinbl = string.Empty;
        private static string terrain_heightmap = string.Empty;
        private static string terrain_texture = string.Empty;
        private static string detailmap3 = string.Empty;
        private static string irrlicht2_bk = string.Empty;
        private static string irrlicht2_dn = string.Empty;
        private static string irrlicht2_ft = string.Empty;
        private static string irrlicht2_lf = string.Empty;
        private static string irrlicht2_rt = string.Empty;
        private static string irrlicht2_up = string.Empty;
        private static string skydome = string.Empty;
        private static string map_20kdm2 = string.Empty;
        private static string fireball = string.Empty;
        private static string particlewhite = string.Empty;

        private static Primitive CreateArrayMap(Primitive array)
        {
            array[0] = array[0];
            return array;
        }

        private static void copyResources()
		{
			try
			{
 			    ninja = resourceFile(Resources.ninja, "ninja.b3d");
			    nskinrd = resourceFile(Resources.nskinrd, "nskinrd.jpg");
                nskinbl = resourceFile(Resources.nskinbl, "nskinbl.jpg");
			    terrain_heightmap = resourceFile(Resources.terrain_heightmap, "terrain_heightmap.bmp");
			    terrain_texture = resourceFile(Resources.terrain_texture, "terrain_texture.jpg");
                detailmap3 = resourceFile(Resources.detailmap3, "detailmap3.jpg");
                irrlicht2_bk = resourceFile(Resources.irrlicht2_bk, "irrlicht2_bk.jpg");
                irrlicht2_dn = resourceFile(Resources.irrlicht2_dn, "irrlicht2_dn.jpg");
                irrlicht2_ft = resourceFile(Resources.irrlicht2_ft, "irrlicht2_ft.jpg");
                irrlicht2_lf = resourceFile(Resources.irrlicht2_lf, "irrlicht2_lf.jpg");
                irrlicht2_rt = resourceFile(Resources.irrlicht2_rt, "irrlicht2_rt.jpg");
                irrlicht2_up = resourceFile(Resources.irrlicht2_up, "irrlicht2_up.jpg");
                skydome = resourceFile(Resources.skydome, "skydome.jpg");
                map_20kdm2 = resourceFile(Resources.map_20kdm2, "map_20kdm2.pk3");
                fireball = resourceFile(Resources.fireball, "fireball.bmp");
                particlewhite = resourceFile(Resources.particlewhite, "particlewhite.bmp");
            }
            catch (Exception e)
            {
                Console.WriteLine(e.Message);
            }
        }

        private static void deleteResources()
		{
			try
			{
                FileIO.Delete(ninja);
                FileIO.Delete(nskinrd);
                FileIO.Delete(nskinbl);
                FileIO.Delete(terrain_heightmap);
                FileIO.Delete(terrain_texture);
                FileIO.Delete(detailmap3);
                FileIO.Delete(irrlicht2_bk);
                FileIO.Delete(irrlicht2_dn);
                FileIO.Delete(irrlicht2_ft);
                FileIO.Delete(irrlicht2_lf);
                FileIO.Delete(irrlicht2_rt);
                FileIO.Delete(irrlicht2_up);
                FileIO.Delete(skydome);
                FileIO.Delete(map_20kdm2);
                FileIO.Delete(fireball);
                FileIO.Delete(particlewhite);
            }
            catch (Exception e)
            {
                Console.WriteLine(e.Message);
            }
        }

        private static string resourceFile(byte[] resource, string fileName)
        {
            string filePath = Path.GetTempPath() + fileName;
            using (Stream stream = FileIO.Create(filePath))
            {
                stream.Write(resource, 0, resource.Length);
                stream.Flush();
                stream.Close();
            }
            return filePath;
        }

        private static string resourceFile(Bitmap resource, string fileName)
        {
            string filePath = Path.GetTempPath() + fileName;
            resource.Save(filePath);
            return filePath;
        }

        private static int keymap(string key)
        {
            int _key = (int)kc.ConvertFromString(key);
            if (_key == 6) return 0x0D;
            else if (_key == 18) return 0x20;
            else if (_key >= 34 && _key <= 43) return _key + (0x30) - 34;
            else if (_key >= 44 && _key <= 69) return _key + (0x41) - 44;
            else if (_key >= 90 && _key <= 113) return _key + (0x70) - 90;
            else if (_key >= 114 && _key <= 121) return _key + (0x90) - 114;
            else return 0;
        }

        /// <summary>
        /// Do the timer callback asynchronously to keep the animation going, and prevent clashing if the callback is not finished before the next call.
        /// </summary>
        private static void timerThread()
        {
            workingTimer = true;
            TimerDelegate();
            workingTimer = false;
        }

        private static void collisionThread()
        {
            workingCollision = true;
            CollisionDelegate();
            workingCollision = false;
        }

        private static void clickThread()
        {
            workingClick = true;
            ClickDelegate();
            workingClick = false;
        }

        private static int[] getRGB(string colour)
        {
            int []RGB = new int[3];
            RGB[0] = (int)((System.Windows.Media.Color)System.Windows.Media.ColorConverter.ConvertFromString(colour)).R;
            RGB[1] = (int)((System.Windows.Media.Color)System.Windows.Media.ColorConverter.ConvertFromString(colour)).G;
            RGB[2] = (int)((System.Windows.Media.Color)System.Windows.Media.ColorConverter.ConvertFromString(colour)).B;
            return RGB;
        }

        /// <summary>
        /// The first object that collided.
        /// </summary>
        public static Primitive CollisionObject1
        {
            get
            {
                return collisionObject1;
            }
        }

        /// <summary>
        /// The second object that collided.
        /// </summary>
        public static Primitive CollisionObject2
        {
            get
            {
                return collisionObject2;
            }
        }

        /// <summary>
        /// The last object that was clicked.
        /// </summary>
        public static Primitive ClickObject
        {
            get
            {
                return clickObject;
            }
        }

        /// <summary>
        /// Whether to output errors and warnings to the console "True" or "False".
        /// </summary>
        public static Primitive ConsoleMessages
        {
            get
            {
                return consoleMessages;
            }
            set
            {
                consoleMessages = value;
                Engine.setConsoleMessages(consoleMessages);
            }
        }

        [HideFromIntellisense]
        public static void Message(int value, StringBuilder strValue1, StringBuilder strValue2)
        {
            if (value == 0 && null != TimerDelegate && !workingTimer)
            {
                Thread thread = new Thread(timerThread);
                thread.Start();
            }
            else if (value == 1 && null != CollisionDelegate && !workingCollision)
            {
                collisionObject1 = strValue1.ToString();
                collisionObject2 = strValue2.ToString();
                Thread thread = new Thread(collisionThread);
                thread.Start();
            }
            else if (value == 2 && null != ClickDelegate && !workingClick)
            {
                clickObject = strValue1.ToString();
                Thread thread = new Thread(clickThread);
                thread.Start();
            }
            else if (value == 99)
            {
                Console.WriteLine(strValue1.ToString() + " " + strValue2.ToString());
            }
        }

        /// <summary>
        /// An event that occurs every 100ms to control the game-play.
        /// </summary>
        public static event SmallBasicCallback Timer
        {
            add
            {
                TimerDelegate = value;
            }
            remove
            {
                TimerDelegate = null;
            }
        }

        /// <summary>
        /// An event that occurs when two objects collide.
        /// To prevent this event from firing multiple times, a short delay (say 100ms) is recommended at the end of this event subroutine.
        /// The object names are obtained from CollisionObject1 and CollisionObject2 in the order they were added.
        /// Only objects added are detected, not for example collision with terrain.
        /// </summary>
        public static event SmallBasicCallback Collision
        {
            add
            {
                CollisionDelegate = value;
            }
            remove
            {
                CollisionDelegate = null;
            }
        }

        /// <summary>
        /// An event that occurs when an object in front of the camera is clicked with the mouse.
        /// The object name is obtained from ClickObject.
        /// </summary>
        public static event SmallBasicCallback Click
        {
            add
            {
                ClickDelegate = value;
            }
            remove
            {
                ClickDelegate = null;
            }
        }

        /// <summary>
        /// Was the named key down since it was last checked.
        /// A key down state will only be reset to "False" after it is checked explicitly with this command or ResetAllKeyDown() is called.
        /// Only alpha-numeric, function, space, return and shift/control keys supported.
        /// </summary>
        /// <param name="key">The key name.</param>
        /// <returns>"True" or "False"</returns>
        public static Primitive WasKeyDown(Primitive key)
        {
            if (!running) return "False";
            return Engine.wasKeyDown(keymap(key));
        }

        /// <summary>
        /// Reset the down state of all keys to "False".
        /// </summary>
        public static void ResetAllKeyDown()
        {
            if (running) return;
            Engine.resetAllKeyDown();
        }

        /// <summary>
        /// Setup the 3D environment (called before all other LD3D commands).
        /// </summary>
        /// <param name="fullScreen">"True" or "False".</param>
        /// <param name="colour">The background (sky) colour.</param>
        /// <returns>The added camera object name.</returns>
        public static Primitive Setup(Primitive fullScreen, Primitive colour)
        {
            try
            {
                if (running) return "";
                copyResources();
                Node node = new Node(getNodeID(), "Camera");
                int[] RGB = getRGB(colour);
                Engine.setup(node.name, fullScreen, 640, 480, RGB[0], RGB[1], RGB[2]);
                Nodes.Add(node);
                running = true;
                return node.name;
            }
            catch (Exception e)
            {
                Console.WriteLine(e.Message);
                return "";
            }
        }

        /// <summary>
        /// Start LD3D Irrlicht display.
        /// </summary>
        public static void Start()
        {
            try
            {
                if (!running) return;
                Engine.start();
                deleteResources();
            }
            catch (Exception e)
            {
                Console.WriteLine(e.Message);
            }
        }

        /// <summary>
        /// Add a skybox.  6 images are required.
        /// </summary>
        /// <param name="up">The up image.</param>
        /// <param name="down">The down image.</param>
        /// <param name="left">The left image.</param>
        /// <param name="right">The right image.</param>
        /// <param name="front">The front image.</param>
        /// <param name="back">The back image.</param>
        /// <returns></returns>
	    public static void AddSkybox(Primitive up, Primitive down, Primitive left, Primitive right, Primitive front, Primitive back)
	    {
            if (!running) return;
            Engine.addSkybox(up, down, left, right, front, back);
	    }

        /// <summary>
        /// Add a default mountain skybox.
        /// </summary>
        /// <returns></returns>
        public static void AddMountainSkybox()
        {
            if (!running) return;
            Engine.addSkybox(irrlicht2_up, irrlicht2_dn, irrlicht2_lf, irrlicht2_rt, irrlicht2_ft, irrlicht2_bk);
        }

        /// <summary>
        /// Add a skydome.
        /// </summary>
        /// <param name="sky">Image for the skydome (top half of image is above the horizon, the bottom half is below the horizon).</param>
        /// <returns></returns>
        public static void AddSkydome(Primitive sky)
        {
            if (!running) return;
            Engine.addSkydome(sky);
        }

        /// <summary>
        /// Add a default cloudy skydome.
        /// </summary>
        /// <returns></returns>
        public static void AddCloudySkydome()
        {
            if (!running) return;
            Engine.addSkydome(skydome);
        }

        /// <summary>
        /// Add a general cube object.
        /// </summary>
        /// <returns>The added object name.</returns>
        public static Primitive AddCube()
        {
            if (!running) return "";
            Node node = new Node(getNodeID());
            Engine.addCube(node.name);
            Nodes.Add(node);
            return node.name;
        }

        /// <summary>
        /// Add a general sphere object.
        /// </summary>
        /// <returns>The added object name.</returns>
        public static Primitive AddSphere()
        {
            if (!running) return "";
            Node node = new Node(getNodeID());
            Engine.addSphere(node.name);
            Nodes.Add(node);
            return node.name;
        }

        /// <summary>
        /// Remove an object.
        /// </summary>
        /// <param name="name">The object name.</param>
        /// <returns></returns>
        public static void Remove(Primitive name)
        {
            if (!running) return;
            Node node = getNode(name);
            if (null == node) return;
            Nodes.Remove(node);
            Engine.remove(name);
        }

        /// <summary>
        /// Add a general animated mesh object (*.b3d).
        /// </summary>
        /// <param name="mesh"></param>
        /// <param name="startFrame">The starting frame for the animated object.</param>
        /// <param name="endFrame">The end fame for the animated object.</param>
        /// <param name="fps">The frames per second for the mesh animation.</param>
        /// <returns>The added object name.</returns>
        public static Primitive AddAnimatedMesh(Primitive mesh, Primitive startFrame, Primitive endFrame, Primitive fps)
        {
            if (!running) return "";
            Node node = new Node(getNodeID());
            Engine.addAnimatedMesh(node.name, mesh, startFrame, endFrame, fps);
            Nodes.Add(node);
            return node.name;
        }

        /// <summary>
        /// A default nija animated mesh object.
        /// </summary>
        /// <param name="colour">"Red" or "Blue"</param>
        /// <returns>The added object name.</returns>
        public static Primitive AddNinja(Primitive colour)
        {
            if (!running) return "";
            Node node = new Node(getNodeID());
            Engine.addAnimatedMesh(node.name, ninja, 0, 13, 10);
            if (colour == "Red")
            {
                Engine.setTexture(node.name, nskinrd);
            }
            else if (colour == "Blue")
            {
                Engine.setTexture(node.name, nskinbl);
            }
            Nodes.Add(node);
            return node.name;
        }

        /// <summary>
        /// Add a general terrain.
        /// </summary>
        /// <param name="height">A heght bitmap (*.bmp)</param>
        /// <param name="terrain">The main terain image (*.jpg).</param>
        /// <param name="detail">A detail terrain image (*.jpg).</param>
        /// <param name="gravity">Gravity of player (0 to fly, 10 for normal gravity).</param>
        /// <returns>The added object name.</returns>
        public static Primitive AddTerrain(Primitive height, Primitive terrain, Primitive detail, Primitive gravity)
        {
            if (!running) return "";
            System.Drawing.Bitmap bm = new System.Drawing.Bitmap(height);
            Node node = new Node(getNodeID(), "Terrain");
            Engine.addTerrain(node.name, height, terrain, detail, gravity, bm.Size.Width, bm.Size.Height);
            Nodes.Add(node);
            return node.name;
        }

        /// <summary>
        /// A default terrain.
        /// </summary>
        /// <param name="gravity">Gravity of player (0 to fly, 10 for normal gravity).</param>
        /// <returns>The added object name.</returns>
        public static Primitive AddMountainTerrain(Primitive gravity)
        {
            if (!running) return "";
            System.Drawing.Bitmap bm = new System.Drawing.Bitmap(terrain_heightmap);
            Node node = new Node(getNodeID(), "Terrain");
            Engine.addTerrain(node.name, terrain_heightmap, terrain_texture, detailmap3, gravity, bm.Size.Width, bm.Size.Height);
            Engine.addSkybox(irrlicht2_up, irrlicht2_dn, irrlicht2_lf, irrlicht2_rt, irrlicht2_ft, irrlicht2_bk);
            Nodes.Add(node);
            return node.name;
        }

        /// <summary>
        /// Add a zip for mesh geometries.
        /// </summary>
        /// <param name="name">The zip mesh file (*.pk3)</param>
        /// <returns></returns>
        public static void AddZip(Primitive name)
        {
            if (!running) return;
            Engine.addZip(name);
        }

        /// <summary>
        /// A Quake mesh file within zip mesh file.
        /// </summary>
        /// <param name="mesh">The Quake mesh file (*.bsp)</param>
        /// <param name="gravity">Gravity of player (0 to fly, 10 for normal gravity).</param>
        /// <returns>The added object name.</returns>
        public static Primitive AddQuakeMesh(Primitive mesh, Primitive gravity)
        {
            if (!running) return "";
            Node node = new Node(getNodeID(), "Quake");
            Engine.addQuakeMesh(node.name, mesh, gravity);
            Nodes.Add(node);
            return node.name;
        }

        /// <summary>
        /// A default Quake mesh.
        /// </summary>
        /// <param name="gravity">Gravity of player (0 to fly, 10 for normal gravity).</param>
        /// <returns>The added object name.</returns>
        public static Primitive AddDungeonMesh(Primitive gravity)
        {
            if (!running) return "";
            Engine.addZip(map_20kdm2);
            Node node = new Node(getNodeID(), "Quake");
            Engine.addQuakeMesh(node.name, "20kdm2.bsp", gravity);
            Nodes.Add(node);
            return node.name;
        }

        /// <summary>
        /// Set the texture on an object.
        /// </summary>
        /// <param name="name">The object name.</param>
        /// <param name="texture">The texture image (*.bmp).</param>
        /// <returns></returns>
        public static void SetTexture(Primitive name, Primitive texture)
        {
            if (!running) return;
            if (null == getNode(name)) return;
            Engine.setTexture(name, texture);
        }

        /// <summary>
        /// Set the position of an object.
        /// </summary>
        /// <param name="name">The object name.</param>
        /// <param name="X">left right position.</param>
        /// <param name="Y">down up position.</param>
        /// <param name="Z">near far position.</param>
        /// <returns></returns>
        public static void SetPosition(Primitive name, Primitive X, Primitive Y, Primitive Z)
        {
            if (!running) return;
            if (null == getNode(name)) return;
            Engine.setPosition(name, X, Y, Z);
        }

        /// <summary>
        /// Get the position of an object.
        /// </summary>
        /// <param name="name">The object name.</param>
        /// <returns>Array of X, Y, Z coordinates.</returns>
        public static Primitive GetPosition(Primitive name)
        {
            if (!running) return "";
            if (null == getNode(name)) return "";
            return CreateArrayMap(Engine.getPosition(name));
        }

        /// <summary>
        /// Set the scale of an object.
        /// </summary>
        /// <param name="name">The object name.</param>
        /// <param name="X">left right scale.</param>
        /// <param name="Y">down up scale.</param>
        /// <param name="Z">near far scale.</param>
        /// <returns></returns>
        public static void SetScale(Primitive name, Primitive X, Primitive Y, Primitive Z)
        {
            if (!running) return;
            if (null == getNode(name)) return;
            Engine.setScale(name, X, Y, Z);
        }

        /// <summary>
        /// Set the rotation of an object.
        /// </summary>
        /// <param name="name">The object name.</param>
        /// <param name="X">left right rotation (degrees).</param>
        /// <param name="Y">down up rotation (degrees).</param>
        /// <param name="Z">near far rotation (degrees).</param>
        /// <returns></returns>
        public static void SetRotation(Primitive name, Primitive X, Primitive Y, Primitive Z)
        {
            if (!running) return;
            if (null == getNode(name)) return;
            Engine.setRotation(name, X, Y, Z);
        }

        /// <summary>
        /// Get the ground level.
        /// </summary>
        /// <param name="X">left right position.</param>
        /// <param name="Z">near far position.</param>
        /// <returns>The ground level.</returns>
        public static Primitive GetTerrainHeight(Primitive X, Primitive Z)
        {
            if (!running) return "";
            return Engine.getTerrainHeight(X, Z);
        }

        /// <summary>
        /// Set the visibility of an object.
        /// </summary>
        /// <param name="name">The object name.</param>
        /// <param name="visible">"True" or "False"</param>
        /// <returns></returns>
        public static void SetVisible(Primitive name, Primitive visible)
        {
            if (!running) return;
            if (null == getNode(name)) return;
            Engine.setVisible(name, visible);
        }

        /// <summary>
        /// Animate an object to absolute cooordinates.
        /// </summary>
        /// <param name="name">The object name.</param>
        /// <param name="X">left right position.</param>
        /// <param name="Y">down up position.</param>
        /// <param name="Z">near far position.</param>
        /// <param name="time">the animation time (ms).</param>
        /// <returns></returns>
        public static void AnimateTo(Primitive name, Primitive X, Primitive Y, Primitive Z, Primitive time)
        {
            if (!running) return;
            if (null == getNode(name)) return;
            Engine.animateTo(name, X, Y, Z, time);
        }

        /// <summary>
        /// Animate an object relative to its current position.
        /// </summary>
        /// <param name="name">The object name.</param>
        /// <param name="X">left right position.</param>
        /// <param name="Y">down up position.</param>
        /// <param name="Z">near far position.</param>
        /// <param name="time">the animation time (ms).</param>
        /// <returns></returns>
        public static void AnimateRelative(Primitive name, Primitive X, Primitive Y, Primitive Z, Primitive time)
        {
            if (!running) return;
            if (null == getNode(name)) return;
            Engine.animateRelative(name, X, Y, Z, time);
        }

        /// <summary>
        /// Animate an object to absolute cooordinates while remaining on the ground.
        /// </summary>
        /// <param name="name">The object name.</param>
        /// <param name="X">left right position.</param>
        /// <param name="Z">near far position.</param>
        /// <param name="time">the animation time (ms).</param>
        /// <returns></returns>
        public static void AnimateOnTerrainTo(Primitive name, Primitive X, Primitive Z, Primitive time)
        {
            if (!running) return;
            if (null == getNode(name)) return;
            Engine.animateOnTerrainTo(name, X, Z, time);
        }

        /// <summary>
        /// Animate an object relative to its current position while remaining on the ground.
        /// </summary>
        /// <param name="name">The object name.</param>
        /// <param name="X">left right position.</param>
        /// <param name="Z">near far position.</param>
        /// <param name="time">the animation time (ms).</param>
        /// <returns></returns>
        public static void AnimateOnTerrainRelative(Primitive name, Primitive X, Primitive Z, Primitive time)
        {
            if (!running) return;
            if (null == getNode(name)) return;
            Engine.animateOnTerrainRelative(name, X, Z, time);
        }

        /// <summary>
        /// Rotate an object continously.
        /// </summary>
        /// <param name="name">The object name.</param>
        /// <param name="X">Rotation about X axis in degrees per second.</param>
        /// <param name="Y">Rotation about Y axis in degrees per second.</param>
        /// <param name="Z">Rotation about Z axis in degrees per second.</param>
        /// <returns></returns>
        public static void AnimateRotation(Primitive name, Primitive X, Primitive Y, Primitive Z)
        {
            if (!running) return;
            if (null == getNode(name)) return;
            Engine.animateRotation(name, X, Y, Z);
        }

        /// <summary>
        /// Animate the onject to fly in a horizontal circle about its center with input radius.
        /// </summary>
        /// <param name="name">The object name.</param>
        /// <param name="radius">The radius of the rotation.</param>
        /// <param name="speed">The speed of the rotation (degrees/second).</param>
        public static void AnimateCircle(Primitive name, Primitive radius, Primitive speed)
        {
            if (!running) return;
            if (null == getNode(name)) return;
            Engine.animateCircle(name, radius, speed);
       }

        /// <summary>
        /// Remove all animations from object.
        /// </summary>
        /// <param name="name">The object name.</param>
        /// <returns></returns>
        public static void RemoveAnimators(Primitive name)
        {
            if (!running) return;
            if (null == getNode(name)) return;
            Engine.removeAnimators(name);
        }

        /// <summary>
        /// Set gravity for an object
        /// </summary>
        /// <param name="name">The object name.</param>
        /// <param name="gravity">Gravity of object (0 to fly, 10 for normal gravity).</param>
        /// <returns></returns>
        public static void SetGravity(Primitive name, Primitive gravity)
        {
            if (!running) return;
            if (null == getNode(name)) return;
            Engine.setGravity(name, gravity);
        }

        /// <summary>
        /// Get the position of the camera.
        /// </summary>
        /// <returns>Array of X, Y, Z coordinates.</returns>
        public static Primitive GetCameraPosition()
        {
            if (!running) return "";
            return Engine.getPosition();
        }

        /// <summary>
        /// Get the view direction of the camera normalised to unit length.
        /// </summary>
        /// <returns>Array of X, Y, Z vector.</returns>
        public static Primitive GetCameraDirection()
        {
            if (!running) return "";
            return Engine.getDirection();
        }

        /// <summary>
        /// Set an object to glow.
        /// </summary>
        /// <param name="name">The object name.</param>
        /// <param name="colour">The glow colour.</param>
        /// <param name="radius">The radius of the glow.</param>
        /// <returns></returns>
        public static void SetGlow(Primitive name, Primitive colour, Primitive radius)
        {
            if (!running) return;
            if (null == getNode(name)) return;
            int[] RGB = getRGB(colour);
            Engine.setGlow(name, RGB[0], RGB[1], RGB[2], radius);
        }

        /// <summary>
        /// Add a light.
        /// </summary>
        /// <param name="X">The light X coordinate.</param>
        /// <param name="Y">The light Y coordinate.</param>
        /// <param name="Z">The light Z coordinate.</param>
        /// <param name="colour">The light colour.</param>
        /// <param name="radius">The radius of the light.</param>
        /// <returns>The added object name.</returns>
        public static Primitive AddLight(Primitive X, Primitive Y, Primitive Z, Primitive colour, Primitive radius)
        {
            if (!running) return "";
            Node node = new Node(getNodeID());
            int[] RGB = getRGB(colour);
            Engine.addLight(node.name, X, Y, Z, RGB[0], RGB[1], RGB[2], radius, fireball, particlewhite);
            Nodes.Add(node);
            return node.name;
        }

        /// <summary>
        /// Ambient light level and colour (affects the brightness of objects not directly lit by a light).
        /// </summary>
        /// <param name="colour">The ambient colour (default white).</param>
        public static void AmbientLight(Primitive colour)
        {
            if (!running) return ;
            int[] RGB = getRGB(colour);
            Engine.ambientLight(RGB[0], RGB[1], RGB[2]);
        }

        /// <summary>
        /// Add a fog effect.
        /// </summary>
        /// <param name="colour">The fog colour</param>
        /// <param name="density">The fog density.</param>
        public static void AddFog(Primitive colour, Primitive density)
        {
            if (!running) return;
            int[] RGB = getRGB(colour);
            Engine.addFog(RGB[0], RGB[1], RGB[2], density);
        }

        /// <summary>
        /// Set size of specular highlights on an object (shininess).
        /// </summary>
        /// <param name="name">The object name.</param>
        /// <param name="shininess">The shininess, 20 is a good value (0.5 to 128).</param>
        public static void SetShininess(Primitive name, Primitive shininess)
        {
            if (!running) return;
            if (null == getNode(name)) return;
            Engine.setShininess(name, shininess);
        }

        /// <summary>
        /// Set some properties of the FPS camera.
        /// </summary>
        /// <param name="speed">The speed (default 500).</param>
        /// <param name="farfield">The farfield view distance (default 10000).</param>
        /// <param name="gravity">Gravity of player (0 to fly, 10 for normal gravity).</param>
        public static void SetCameraProperties(Primitive speed, Primitive farfield, Primitive gravity)
		{
            if (!running) return;
            Engine.setCameraProperties(speed, farfield, gravity);
		}

        /// <summary>
        /// Add a 2D billboard that always faces the camera.
        /// </summary>
        /// <param name="width">The width of the billboard.</param>
        /// <param name="height">The height of the billboard.</param>
        /// <returns>The added object name.</returns>
        public static Primitive AddBillBoard(Primitive width, Primitive height)
		{
            if (!running) return "";
            Node node = new Node(getNodeID());
            Engine.addBillBoard(node.name, width, height);
            Nodes.Add(node);
            return node.name;
		}

        /// <summary>
        /// Get a string describing the object type.
        /// </summary>
        /// <param name="name">The object name.</param>
        /// <returns>The object type description.</returns>
        public static Primitive GetType(Primitive name)
        {
            if (!running) return "";
            Node node = getNode(name);
            if (null == node) return "";
            if (node.type == "") node.type = Engine.getType(name);
            return node.type;
        }

        /// <summary>
        /// Set a string describing the object type to replace the default.
        /// Useful to detect specific combinations of collisions for example.
        /// </summary>
        /// <param name="name">The object name.</param>
        /// <param name="type">The object type description.</param>
        /// <returns></returns>
        public static void SetType(Primitive name, Primitive type)
        {
            if (!running) return;
            Node node = getNode(name);
            if (null == node) return;
            node.type = type;
        }
    }
}
