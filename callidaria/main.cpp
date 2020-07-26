#define GLM_ENABLE_EXPERIMENTAL 1
#include <iostream>
#include "cld_lin/frm/frame.h"
#include "cld_lin/gfx/renderer2d.h"
#include "cld_lin/gfx/renderer3d.h"
#include "cld_lin/gfx/rendereri.h"
#include "cld_lin/mat/camera2d.h"
#include "cld_lin/mat/camera3d.h"
#include "cld_lin/gfx/light3d.h"
#include "cld_lin/gfx/light_point3d.h"
#include "cld_lin/gfx/light_spot3d.h"
#include "cld_lin/gfx/material3d.h"
#include "cld_lin/ppe/msaa.h"
#include "cld_lin/ppe/bloom.h"
#include "cld_lin/fcn/text.h"
#include "cld_lin/aud/audio.h"
#include "cld_lin/aud/listener.h"
#include "cld_lin/fcn/terrain.h"
#include "cld_lin/gfx/cubemap.h"

int main(int argc,char** argv)
{
	Frame f = Frame("callidaria",1,1280,720,false);

	// AUDIO
	Listener listener=Listener();
	Audio bgm = Audio("./res/audio.wav");
	Audio nw_sfx = Audio("./res/nice-work.wav",1,1,glm::vec3(0,0,0),glm::vec3(0,0,0),false);

	// RENDERERS
	Renderer2D r2d = Renderer2D();
	RendererI ri = RendererI();
	Renderer3D r3d = Renderer3D();

	// OBJECTS
	r2d.add(glm::vec2(100,100),100,100,"./res/testbild.png");
	r2d.add(glm::vec2(150,150),100,100,"./res/grass.png");

	r3d.add("res/sun.obj","res/black.png","res/black.png","res/black.png","res/sun_emit.png",
			glm::vec3(-200,100,-250),10,glm::vec3(0,0,0));
	r3d.add("res/floor.obj","res/mat/pav_stones/alb.jpg","res/mat/pav_stones/disp.jpg",
			"res/mat/pav_stones/norm.jpg","res/black.png",glm::vec3(0,0,0),2.5f,glm::vec3(0,0,0));
	r3d.add("res/nsphere.obj","res/mat/marble/alb.jpg","res/mat/marble/disp.jpg","res/mat/marble/norm.jpg",
			"res/black.png",glm::vec3(4,3,-5),1,glm::vec3(0,0,0));
	r3d.add("res/nsphere.obj","res/mat/wood/alb.jpg","res/mat/wood/rough.jpg","res/mat/wood/norm.jpg",
			"res/black.png",glm::vec3(-2.3f,5.5f,-3),1,glm::vec3(0,0,0));
	r3d.add("res/nsphere.obj","res/mat/metal/alb.jpg","res/mat/metal/disp.jpg","res/mat/metal/norm.jpg",
			"res/black.png",glm::vec3(0,2,0),1,glm::vec3(0,0,0));

	// CAMERAS
	Camera2D cam2d=Camera2D(1920.0f,1080.0f);
	Camera3D cam3d=Camera3D(glm::vec3(-4,4,-7),1920.0f,1080.0f,90.0f);
	r2d.load_wcam(&cam2d);ri.load_wcam(&cam2d);r3d.load(&cam3d);

	// TERRAIN
	//Terrain trn = Terrain(&cam3d,glm::vec3(-250,0,-250),500,500,"res/trntex.jpg","res/heightmap.bmp",25);

	// LIGHTS
	Light3D l0=Light3D(&r3d,0,glm::vec3(-200,100,-250),glm::vec3(1,1,1),1);
	l0.upload();l0.set_amnt(1);
	l0.create_shadow(glm::vec3(0,0,0),50,50,5,4096);

	// MATERIALS
	Material3D m0=Material3D(&r3d,3,8,0.25f);
	Material3D m1=Material3D(&r3d,1,64,2.0f);

	// POST PROCESSING
	Bloom bloom=Bloom(&f);
	MSAA msaa=MSAA("shader/fbv_standard.shader","shader/fbf_standard.shader",16);
	FrameBuffer ifb=FrameBuffer(f.w_res,f.h_res,"shader/fbv_standard.shader","shader/fbf_standard.shader",false);

	// TEXT
	Font fnt=Font("res/fonts/nimbus_roman.fnt","res/fonts/nimbus_roman.png",30,30);Text tft=Text(&fnt);
	tft.add("callidaria FRAMEWORK",glm::vec2(20,940));
	tft.add("this is a test",glm::vec2(20,1040));
	tft.add("lorem Ipsum",glm::vec2(20,1020));
	tft.add("AaBbCcDdEeFfGgHhIiJjKkLlMmNnOoPp",glm::vec2(20,1000));
	tft.add("QqRrSsTtUuVvWwXxYyZz",glm::vec2(20,980));
	tft.load_wcam(&cam2d);tft.load_text();

	// CUBEMAP
	std::vector<const char*> cmtex = {
		"res/skybox/right.jpg",
		"res/skybox/left.jpg",
		"res/skybox/top.jpg",
		"res/skybox/bottom.jpg",
		"res/skybox/front.jpg",
		"res/skybox/back.jpg"
	};
	Cubemap cm = Cubemap(cmtex);

	// PPE
	Bloom blm = Bloom(&f);

	float pitch=0;float yaw=45.0f;int lfx,lfy;glm::mat4 ml=glm::mat4(1.0f);
	int flow_tex=0;

	bgm.play();

	bool run=true;while (run) {
		f.vsync(60);f.input(run);

		// INPUT
		if (f.kb.ka[SDLK_ESCAPE]) break;
		if (f.mouse.mcl) {
			pitch+=(f.mouse.my-lfy)*-0.1f;
			yaw+=(f.mouse.mx-lfx)*0.1f;
		} lfx=f.mouse.mx;lfy=f.mouse.my;
		if (f.kb.ka[SDLK_i]) pitch+=1.0f;
		else if (f.kb.ka[SDLK_k]) pitch-=1.0f;
		if (f.kb.ka[SDLK_j]) yaw-=1.0f;
		else if (f.kb.ka[SDLK_l]) yaw+=1.0f;
		cam3d.front.x=cos(glm::radians(pitch))*cos(glm::radians(yaw));
		cam3d.front.y=sin(glm::radians(pitch));
		cam3d.front.z=cos(glm::radians(pitch))*sin(glm::radians(yaw));
		cam3d.front=glm::normalize(cam3d.front);
		if (f.kb.ka[SDLK_w]) cam3d.pos+=0.05f*cam3d.front;
		else if (f.kb.ka[SDLK_s]) cam3d.pos-=0.05f*cam3d.front;
		if (f.kb.ka[SDLK_a]) cam3d.pos-=0.05f*glm::normalize(glm::cross(cam3d.front,cam3d.up));
		else if (f.kb.ka[SDLK_d]) cam3d.pos+=0.05f*glm::normalize(glm::cross(cam3d.front,cam3d.up));
		if (f.kb.ka[SDLK_r]) cam3d.pos+=0.05f*cam3d.up;
		else if (f.kb.ka[SDLK_f]) cam3d.pos-=0.05f*cam3d.up;

		for (int i=0;i<f.xb.size();i++) {
			if (f.xb.at(i).xbb[SDL_CONTROLLER_BUTTON_A]) printf("\033[0mbutton a%i pressed\n",i);
			if (f.xb.at(i).xba[SDL_CONTROLLER_AXIS_TRIGGERRIGHT]>1000)
				printf("\033[0mtrigger%i casted\n",i);
		}

		// SHADOW
		l0.prepare_shadow();
		m1.upload();r3d.render_mesh(2,5);
		l0.close_shadow(f.w_res,f.h_res);

		// RENDER
		//msaa.bind();
		//blm.bloom();
		//ifb.bind();
		f.clear(0.1f,0.1f,0.1f);
		glCullFace(GL_BACK);
		r3d.prepare_wcam(&cam3d);
		l0.upload_shadow();
		cm.set_cubemap();
		m0.upload();r3d.render_mesh(1,2);
		m1.upload();r3d.render_mesh(2,5);

		// SKYBOX
		glDisable(GL_CULL_FACE);
		cm.prepare_wcam(&cam3d);
		cm.render();

		//MSAA
		//msaa.blit(&ifb);msaa.close();f.clear(0,0,0);ifb.render();
		//blm.stop();blm.setup();f.clear(0,0,0);blm.render();
		//ifb.close();f.clear(0,0,0);ifb.render();

		r2d.prepare();
		r2d.render_sprite(0,2);

		tft.prepare();
		tft.render(92,glm::vec4(0,0,0,1));

		f.update();
	}
	bgm.remove();
	nw_sfx.remove();

	f.vanish();
	return 0;
}
