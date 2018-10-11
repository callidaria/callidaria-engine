the correct usage of the CALLIDARIA engine

!!!!! DISCLAIMER : SDL2 is required !!!!!

Linux : clone repo, make and go!
Windows : download files and modify them with the editor of your choice


HOW TO USE :

			_the rendering_
- to create sprite/animation use : r.add(glm::vec2 position, float width, float height, const char pointer texture path);
- after creation load the data with : r.load();
- to render first : r.prepare();
- then render with : r.render<_sprite><_anim>(int first, int amount);
- to render everything use : r.render<_sprite><_anim>(0, get_max<_sprite><_anim>())
