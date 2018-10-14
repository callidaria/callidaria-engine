the correct usage of the CALLIDARIA engine

!!!!! DISCLAIMER : SDL2 is required !!!!!

Linux : clone repo, make and go!
Windows : download files and modify them with the editor of your choice


HOW TO USE :

			_the rendering_
- to create sprite/animation use : Renderer.add(glm::vec2 position, float width, float height, const char* texture path);
- after creation load the data with : Renderer.load();
- to render first : Renderer.prepare();
- then render with : Renderer.render<_sprite><_anim>(int first, int amount);
- to render everything use : Renderer.render<_sprite><_anim>(0, get_max<_sprite><_anim>())

            _buttons_
- to create a button use : Button(&Renderer,glm::vec2 position, int width, int height, const char* label, bool next);
- Renderer.load() & Renderer.prepare() are nessessary
- to render use : Button.render(int mouse_position_x, int mouse_position_y, bool mouseclick);
