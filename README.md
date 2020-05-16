<h1>callidaria</h2>
<h2>Setup</h2>

**welcome to the callidaria engine**

**REQUIREMENTS: SDL2, GLEW, GLM, OPENAL AND SOIL**

Linux : clone repo, make and go with cld_lin!
Windows : put cld_win and shader in the same folder as your main.cpp and include!

*windows version is 1.1.3*

<h2>Dokumentation</h2>
<h3>Open Window</h3>

create window:
```c++
//Frame(const char*, int, int, bool);
Frame f = Frame(title,px_width,px_height,fullscreen);
//TODO: setup
```
update loop until closed:
```c++
bool run = true;
while(run) {
    //sets the frames per second to a certain amount
    f.vsync(fps); //optional: vsync(int);
    //updates input for keyboard, mouse and controllers
    f.input(run); //input(bool);
    //TODO: input code
    //clears the screen in the set colour
    f.clear(red,green,blue); //clear(float, float, float);
    //TODO: loop code
    //updates the content of the window
    f.update()
}
```
close window:
```c++
f.vanish();
```
<h3>Input</h3>
check for keyboard input:
```c++
if(f.kb.ka[/*SDLK key code here*/]) //do whatever you want as result
```
*for key codes use sdl2 documentation as a reference*

check for mouse input:
```c++
f.m./*mouse identifier*/
```
mouse identifiers:
```c++
bool mcl; //is true when left mouse button is pressed
int mx,my; //representing x and y position of mouse cursor
```
xbox controller:
```c++
f.xb./*xbox identifier*/
```
xbox identifiers:
```c++
bool start,back; //true when start or option button is pressed
bool a,b,x,y; //representing the face buttons
bool up,down,left,right; //representing the dpad
bool left_sh,right_sh; //representing the shoulder buttons
int left_tr,right_tr; //shows how much the triggers are pressed
int xal,yal; //represents the left control stick
int xar,yar; //represents the right control stick
```
<h3>2D Rendering</h3>
create renderer:
```c++
Renderer2D r2d = Renderer2D();
```
add a sprite:
```c++
//add(glm::vec2, float, float, const char*)
r2d.add(position,width,height,texture_path);
```
add an animation:
```c++
//add(glm::vec2, float, float, const char*, int, int, int, int);
r2d.add(position,width,height,texsheet,rows,columns,frames,tex_amount);
```
upload everything while in setup phase:
```c++
//needs to be done before entering the loop
r2d.load();
//or optional: upload everything together with the camera
r2d.load(&cam2d); //load(Camera2D*);
```
*to create a camera refer to the camera section*

prepare the renderer in loop code:
```c++
//needs to be done before rendering
r2d.prepare();
```
define sprite location:
```c++
//position defined by transformation matrix relative to origin position
r2d.upload_model(model); //upload_model(glm::mat4);
```
render sprite:
```c++
//render sprites from the first parameter until the last parameter
r2d.render_sprite(first_sprite,until_sprite); //render_sprite(int, int);
```
render animation:
```c++
//render animation by id
r2d.render_anim(anim_id); //render_anim(int);
//render specific frame of animation
//frame is defined by its location on the spritesheet
r2d.render_state(anim_id,spritesheet_id); //render_state(int,glm::vec2);
```
getting sprite and animation amount:
```c++
r2d.get_max_sprite(); //return int
r2d.get_max_anim(); //return int
```
reset shader uploads:
```c++
r2d.reset_shader();
```
<h3>3D Rendering</h3>
create renderer:
```c++
Renderer3D r3d = Renderer3D();
```
add a mesh:
```c++
//add(const char*,const char*,const char*,const char*,const char*,glm::vec3,float,glm::vec3);
r3d.add(obj_path,tex_path,specmap,normalmap,emitmap,position,size,rotation);
```
*.obj files supported*

upload everything while in setup phase:
```c++
r3d.load(&cam3d); //load(Camera3D*);
```
*to create a camera refer to the camera section*

prepare the renderer in loop code:
```c++
r3d.prepare();
//or optional: prepare everything and the camera
r3d.prepare_wcam(cam3d); //prepare_wcam(Camera3D*);
```
define mesh position:
```c++
//position defined by transformation matrix relative to origin position
r3d.upload_model(model); //upload_model(glm::mat4);
```
render mesh:
```c++
//meshes rendered from first parameter until the second
r3d.render_mesh(first_mesh,until_mesh); //render_mesh(int,int);
```
upload shadow matrix:
```c++
r3d.upload_shadow(shadow); //upload_shadow(glm::mat4);
```
*to create a shadow refer to the light section*
<h3>Instancing</h3>
TODO
<h3>Camera</h3>
create a 2D camera:
```c++
//Camera2D(float, float);
Camera2D cam2d = Camera2D(width_res,height_res);
```
create a 3D camera:
```c++
//Camera3D(glm::vec3,float,float,float);
Camera3D cam3d = Camera3D(position,width_res,height_res,fov);
```
update 3D camera:
```c++
//after changing camera components use this
cam3d.update();
```
camera components:
```c++
glm::vec3 pos; //representing the camera position
glm::vec3 front; //representing the look vector
glm::vec3 up; //representing up relative relative to the camera
```
<h3>Light</h3>
TODO
<h3>Materials</h3>
create a material:
```c++
//Material3D(Renderer3D*,int,int,float);
Material3D m0 = Material3D(&r3d,tex_repeat,reflect_specular,reflect_intensity);
```
upload material:
```c++
//use right before rendering
m0.upload();
```
<h3>Audio</h3>
create a listener:
```c++
//creates listener at origin position, velocity and orientation
Listener l_aud = Listener();
//or: creates listener at defined parameters
//Listener(glm::vec3,glm::vec3,glm::vec3);
Listener l_aud = Listener(position,velocity,orientation);
```
set listener attributes:
```c++
//set listener position
l_aud.set_position(position); //set_position(glm::vec3);
//set listener velocity
l_aud.set_velocity(velocity); //set_velocity(glm::vec3);
//set listener orientation
l_aud.set_orientation(orientation); //set_orientation(glm::vec3);
```
create audio:
```c++
//Audio(const char*,float,float,glm::vec3,vec3,bool);
Audio aud = Audio(path,gain,pitch,position,velocity,loop);
```
play audio:
```c++
aud.play();
```
set audio attributes:
```c++
//set audio gain
aud.set_gain(gain); //set_gain(float);
//set audio pitch
aud.set_pitch(pitch); //set_pitch(float);
//set audio position
aud.set_position(position); //set_position(glm::vec3);
//set audio velocity
aud.set_velocity(velocity); //set_velocity(glm::vec3);
```
remove audio:
```c++
aud.remove();
```
<h3>Framebuffer</h3>
create a framebuffer:
```c++
//FrameBuffer(int,int,const char*,const char*,bool);
FrameBuffer fb = FrameBuffer(frame_width,frame_height,vertex_shader,fragment_shader,float_buffer);
```
bind the framebuffer:
```c++
//use before rendering to the framebuffer
fb.bind();
```
close the framebuffer:
```c++
//use to stop rendering to the framebuffer
fb.close();
```
render the framebuffer:
```c++
fb.render();
```
get attributes:
```c++
//get the framebuffer object
fb.get_fbo();
//get the framebuffer as texture
fb.get_tex();
```
<h3>Cubemaps & Skyboxes</h3>
create a cubemap:
```c++
//Cubemap(std::vector<const char*>);
Cubemap cm = Cubemap(texture_path);
```
upload cubemap:
```c++
cm.set_cubemap(); //not necessary if rendered as skybox
```
prepare rendering cubemap as skybox:
```c++
cm.prepare();
//or: prepare together with 3D camera
cm.prepare_wcam(cam3d); //prepare(Camera3D*);
```
render cubemap as skybox:
```c++
cm.render();
```
<h3>Text</h3>
TODO
<h3>Post Processing Effects</h3>
TODO
<h5>MSAA</h5>
create msaa effect:
```c++
//MSAA(const char*,const char*,int);
MSAA msaa = MSAA(vertex_shader,fragment_shader,sample_multiplier);
```
enable the msaa effect in loop code:
```c++
msaa.bind(); //use before rendering content that should be multisampled
```
blit msaa to framebuffer:
```c++
msaa.blit(&fb); //blit(FrameBuffer*);
```
stop msaa:
```c++
msaa.close(); //use before rendering msaa
```
render multisampled image:
```c++
msaa.render();
```
<h5>Bloom</h5>
create bloom effect:
```c++
Bloom bloom = Bloom(&f); //Bloom(Frame*);
```
enable the bloom effect:
```c++
bloom.bloom(); //use before rendering bloomed scene
```
stop bloom effect:
```c++
bloom.stop(); //use to stop blooming scene
```
prepare bloom effect:
```c++
bloom.setup(); //use before rendering bloomed image
```
render bloomed image:
```c++
bloom.render();
```
<h5>Blur</h5>
create blur effect:
```c++
Blur blur = Blur(&f); //Blur(Frame*);
```
enable the blur effect:
```c++
blur.blur(); //use before rendering blurred scene
```
stop blur effect:
```c++
blur.stop(); //use before rendering blurred image
```
render blurred image:
```c++
blur.render();
//or: render the blurred to a framebuffer
blur.render_to(&fb); //render_to(FrameBuffer*);
```
<h3>EOF</h3>

**thank you so much for supporting callidaria**