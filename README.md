![play](https://github.com/yoshikiri/TrackMaker/blob/master/resource/thumbnail/play.png)


# TrackMaker
This is a game to control the object moving toward the target and make the track (find the route) that can pass the checkpoint in the shortest time.   
[Demo play movie on YouTube](https://youtu.be/pc5txYNgkhs)

# Dependency
- Ubuntu 18.04
- C++14
- [OpenGL 3.3](https://www.opengl.org/)
- GLSL 3.3
- [GLEW 2.1.0](http://glew.sourceforge.net/)
- [GLFW 3](https://www.glfw.org/)
- [stb_image.h](https://github.com/nothings/stb.git)
- [irrKlang](https://www.ambiera.com/irrklang/)

# Setup
```sh 
$ git clone https://github.com/yoshikiri/TrackMaker.git  
$ cd TrackMaker/src  
$ mkdir build  
$ cd build  
$ cmake ..  
$ make  
$ ./TrackMaker

```

# How to Play
1. Choose stage and press Enter.  
1. Click mouse left button to add targets.  
1. Press Enter to start moving phase.  
1. Check all goals to clear and challenge best time.  

![result](https://github.com/yoshikiri/TrackMaker/blob/master/resource/thumbnail/result.png)

# Licence
This software is released under the MIT License, see LICENSE.

# Credit
- sound: [魔王魂](https://maoudamashii.jokersounds.com/music_rule.html)
- texture loader: [stb_image.h](https://github.com/nothings/stb.git)
- audio library: [irrKlang](https://www.ambiera.com/irrklang/)

# References
- [Learn OpenGL](https://learnopengl.com/)
- [opengl-tutorial](http://www.opengl-tutorial.org/)
- [○○くんのために一所懸命書いたものの結局○○くんの卒業に間に合わなかったGLFWによるOpenGL入門(draft版)](http://marina.sys.wakayama-u.ac.jp/~tokoi/GLFWdraft.pdf)
