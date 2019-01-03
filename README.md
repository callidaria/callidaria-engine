the correct usage of the CALLIDARIA engine

!!!!! DISCLAIMER : SDL2, GLEW, GLM AND SOIL ARE REQUIRED !!!!!

Linux : clone repo, make and go!
Windows : coming soon until then : copy in ide


HOW TO USE :

the example_main.cpp file shows an compilable setup and rendering example code

			_the rendering_
- to create sprite/animation use : Renderer2D.add(glm::vec2 position, float width, float height, const char* texture path);
- after creation load data with : Renderer2D.load();
- to render first : Renderer2D.prepare();
- then render with : Renderer2D.render<_sprite><_anim>(int first, int amount);
- to render everything use : Renderer2D.render<_sprite><_anim>(0, get_max<_sprite><_anim>())

			_buttons_
- to create a button use : Button(&Renderer,glm::vec2 position, int width, int height, const char* label, bool next);
- Renderer2D.load() & Renderer2D.prepare() are nessessary
- to render use : Button.render(int mouse_position_x, int mouse_position_y, bool mouseclick);

			_instance_
- to create instance object use : RendererI.add(glm::vec2 position, int width, int height, const char* texture_path);
- after creation load data with : RendererI.load();
- to set offset values use : RendererI.set_offset(int object_index, int offset_index, glm::vec2 offset);
- to render first : RendererI.prepare();
- then render with : RendererI.render(int instance_index, int amount);

            _the transformation_
- to transform an anim or sprite object : glm::mat4 t = <Sprite><Anim>.transform(glm::vec2 translation, glm::vec2 scale_multiplication, float rotation_angle);
- then upload the matrix with : <Renderer2D>.upload_model(t); when shader is active or renderer is prepared

            _the camera_
- to setup the camera run the constructor : Camera2D cam2d = Camera2D();
- after 2D and instance shaders are compiled run : Camera2D.load(Renderer2D*,RendererI*);
