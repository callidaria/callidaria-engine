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
	Frame f = Frame("callidaria",0,1280,720,(SDL_WindowFlags)0);

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
			"res/black.png",glm::vec3(0,0,0),1,glm::vec3(0,0,0));
	r3d.add("res/nsphere.obj","res/mat/wood/alb.jpg","res/mat/wood/rough.jpg","res/mat/wood/norm.jpg",
			"res/black.png",glm::vec3(0,0,0),1,glm::vec3(0,0,0));
	r3d.add("res/nsphere.obj","res/mat/metal/alb.jpg","res/mat/metal/disp.jpg","res/mat/metal/norm.jpg",
			"res/black.png",glm::vec3(0,0,0),1,glm::vec3(0,0,0));

	// CAMERAS
	Camera2D cam2d=Camera2D(1920.0f,1080.0f);
	Camera3D cam3d=Camera3D(glm::vec3(4,5,-9.5f),1920.0f,1080.0f,90.0f);
	r2d.load_wcam(&cam2d);ri.load_wcam(&cam2d);r3d.load(&cam3d);

	// TERRAIN
	//Terrain trn = Terrain(&cam3d,glm::vec3(-250,0,-250),500,500,"res/trntex.jpg","res/heightmap.bmp",25);

	// LIGHTS
	Light3D l0=Light3D(&r3d,0,glm::vec3(-200,100,-250),glm::vec3(1,1,1),1);
	//Light3D l1=Light3D(&r3d,0,glm::vec3(200,-100,250),glm::vec3(1,1,1),0.2f);
	l0.upload();l0.set_amnt(1);//l1.upload();l0.set_amnt(2);
	l0.create_shadow(glm::vec3(0,0,0),50,50,5,4096);

	// MATERIALS
	Material3D m0=Material3D(&r3d,3,8,0.25f);
	Material3D m1=Material3D(&r3d,1,64,2.0f);

	// POST PROCESSING
	Bloom blm=Bloom(&f);
	MSAA msaa=MSAA("shader/fbv_standard.shader","shader/fbf_standard.shader",16);
	FrameBuffer ifb=FrameBuffer(f.w_res,f.h_res,"shader/fbv_standard.shader","shader/fbf_standard.shader",false);
	Blur blr = Blur(&f);

	// TEXT
	Font fnt=Font("res/fonts/nimbus_roman.fnt","res/fonts/nimbus_roman.png",30,30);
	Text tft=Text(&fnt);
	tft.add("callidaria FRAMEWORK",glm::vec2(20,660));
	tft.add("this is a test",glm::vec2(20,630));
	tft.add("lorem Ipsum",glm::vec2(20,600));
	tft.add("AaBbCcDdEeFfGgHhIiJjKkLlMmNnOoPp",glm::vec2(20,570));
	tft.add("QqRrSsTtUuVvWwXxYyZz",glm::vec2(20,540));
	tft.load_wcam(&cam2d);

	// CUBEMAP
	std::vector<const char*> cmtex = {
		"res/cloudy/graycloud_rt.jpg",
		"res/cloudy/graycloud_lf.jpg",
		"res/cloudy/graycloud_up.jpg",
		"res/cloudy/graycloud_dn.jpg",
		"res/cloudy/graycloud_ft.jpg",
		"res/cloudy/graycloud_bk.jpg"
	}; Cubemap cm = Cubemap(cmtex);

	float pitch=-25;float yaw=120.0f;int lfx,lfy;glm::mat4 ml=glm::mat4(1.0f);
	int flow_tex=0;

	//bgm.play();
	
	glm::vec3 pos_arr[3] = { glm::vec3(4,12,-5),glm::vec3(-2.3f,22,-3),glm::vec3(0,8,0) };
	glm::vec3 mnt_arr[3] = { glm::vec3(0),glm::vec3(0),glm::vec3(0) };

	bool run=true;while (run) {
		f.vsync(60);f.input(run);

		// INPUT
		if (f.kb.ka[SDL_SCANCODE_ESCAPE]) break;
		if (f.mouse.mcl) { pitch+=(f.mouse.my-lfy)*-0.1f;yaw+=(f.mouse.mx-lfx)*0.1f; }
		lfx=f.mouse.mx;lfy=f.mouse.my;

		if (f.kb.ka[SDL_SCANCODE_I]) pitch+=1.0f;
		else if (f.kb.ka[SDL_SCANCODE_K]) pitch-=1.0f;
		if (f.kb.ka[SDL_SCANCODE_J]) yaw-=1.0f;
		else if (f.kb.ka[SDL_SCANCODE_L]) yaw+=1.0f;
		cam3d.front.x=cos(glm::radians(pitch))*cos(glm::radians(yaw));
		cam3d.front.y=sin(glm::radians(pitch));
		cam3d.front.z=cos(glm::radians(pitch))*sin(glm::radians(yaw));
		cam3d.front=glm::normalize(cam3d.front);

		if (f.kb.ka[SDL_SCANCODE_W]) cam3d.pos+=0.05f*cam3d.front;
		else if (f.kb.ka[SDL_SCANCODE_S]) cam3d.pos-=0.05f*cam3d.front;
		if (f.kb.ka[SDL_SCANCODE_A]) cam3d.pos-=0.05f*glm::normalize(glm::cross(cam3d.front,cam3d.up));
		else if (f.kb.ka[SDL_SCANCODE_D]) cam3d.pos+=0.05f*glm::normalize(glm::cross(cam3d.front,cam3d.up));
		if (f.kb.ka[SDL_SCANCODE_R]) cam3d.pos+=0.05f*cam3d.up;
		else if (f.kb.ka[SDL_SCANCODE_F]) cam3d.pos-=0.05f*cam3d.up;

		// PHYSICS PROTOTYPE
		for (int i=0;i<3;i++) {
			for (int j=i+1;j<3;j++) {
				if (glm::length(pos_arr[i]-pos_arr[j])<2)
					printf("%i collided with %i\n",i,j);
			} if (pos_arr[i].y-1<0) {
				pos_arr[i].y = 1;
				mnt_arr[i].y = mnt_arr[i].y*-0.85f;
			} mnt_arr[i].y -= 0.0135f;
			pos_arr[i] += mnt_arr[i];
		}
		glm::mat4 mat_arr[3] = {
			glm::translate(glm::mat4(1.0f),pos_arr[0]),
			glm::translate(glm::mat4(1.0f),pos_arr[1]),
			glm::translate(glm::mat4(1.0f),pos_arr[2])
		};

		// SHADOW
		l0.prepare_shadow();
		m1.upload();
		r3d.upload_model(mat_arr[0]);r3d.render_mesh(2,3);
		r3d.upload_model(mat_arr[1]);r3d.render_mesh(3,4);
		r3d.upload_model(mat_arr[2]);r3d.render_mesh(4,5);
		l0.close_shadow(f.w_res,f.h_res);

		// RENDER
		//msaa.bind();
		//blm.bloom();
		//ifb.bind();
		//blr.blur();
		f.clear(0.1f,0.1f,0.1f);
		glCullFace(GL_BACK);
		r3d.prepare_wcam(&cam3d);
		l0.upload_shadow();
		cm.set_cubemap();
		m0.upload();r3d.upload_model(glm::mat4(1.0f));r3d.render_mesh(1,2);
		m1.upload();
		r3d.upload_model(mat_arr[0]);r3d.render_mesh(2,3);
		r3d.upload_model(mat_arr[1]);r3d.render_mesh(3,4);
		r3d.upload_model(mat_arr[2]);r3d.render_mesh(4,5);

		// SKYBOX
		glDisable(GL_CULL_FACE);
		cm.prepare_wcam(&cam3d);
		cm.render();

		//MSAA
		//blr.stop();f.clear(0.1f,0.1f,0.1f);blr.render();
		//msaa.blit(&ifb);msaa.close();f.clear(0.1f,0.1f,0.1f);msaa.render();
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
