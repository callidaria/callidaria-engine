#define GLM_ENABLE_EXPERIMENTAL 1
#include <iostream>
#include "cld_lin/frm/frame.h"
#include "cld_lin/gfx/renderer2d.h"
#include "cld_lin/gfx/renderer3d.h"
#include "cld_lin/fcn/button.h"
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

int main(int argc, char** argv)
{
	Frame f = Frame();

	Listener listener=Listener();
	Audio bgm=Audio("./res/audio.wav",1,1,glm::vec3(0,0,0),glm::vec3(0,0,0),true);
	Audio nw_sfx=Audio("./res/nice-work.wav",1,1,glm::vec3(0,0,0),glm::vec3(0,0,0),false);
	bgm.play();
    
	Renderer2D r2d = Renderer2D(); RendererI ri = RendererI();
	Renderer3D r3d = Renderer3D();
    
	r2d.add(glm::vec2(100,100),100,100,"./res/testbild.png");
	r2d.add(glm::vec2(150,150),100,100,"./res/grass.png");

	r3d.add("res/sun.obj","res/black.png","res/black.png","res/sun_emit.png",
			glm::vec3(200,100,120),10,glm::vec3(0,0,0));
	r3d.add("res/floor.obj","res/planks.png","res/planks_spec.png",
			"res/black.png",glm::vec3(0,0,0),10,
			glm::vec3(0,0,0));
	r3d.add("res/cube.obj","res/container.png","res/container_spec.png",
			"res/black.png",glm::vec3(0,1,0),1,glm::vec3(0,0,0));
	r3d.add("res/cube.obj","res/container.png","res/container_spec.png",
			"res/black.png",glm::vec3(3,1,4),1,glm::vec3(0,45,0));
	r3d.add("res/sphere.obj","res/weight.png","res/white.png","res/black.png",
			glm::vec3(4,3,-5),1,glm::vec3(0,0,0));
	r3d.add("res/sphere.obj","res/weight.png","res/white.png","res/black.png",
			glm::vec3(2.3f,5.5f,-3),1,glm::vec3(0,0,0));

	Camera2D cam2d=Camera2D();
	Camera3D cam3d=Camera3D(glm::vec3(-4,4,-7));
	r2d.load_wcam(&cam2d);ri.load_wcam(&cam2d);r3d.load(&cam3d);

	Light3D l0=Light3D(&r3d,0,glm::vec3(200,100,120),glm::vec3(1,1,1),1);
	Light3D l1=Light3D(&r3d,1,glm::vec3(-200,-100,-120),
			glm::vec3(0.0f,0.35f,1.0f),0.25f);
	PointLight3D pl0=PointLight3D(&r3d,0,glm::vec3(1,4,1),glm::vec3(1,1,1),1,
			0.09f,0.032f,1);
	SpotLight3D sl0=SpotLight3D(&r3d,0,glm::vec3(1,7,1),glm::vec3(1,1,1),
			glm::vec3(0,-1,0),12.5f,17.5f);
	SpotLight3D sl1=SpotLight3D(&r3d,1,glm::vec3(-3,6,-4),glm::vec3(1,0.3f,1),
			glm::vec3(0,-1,0.05f),15,22.5f);

	l0.upload();l1.upload();pl0.upload();l0.set_amnt(2);pl0.set_amnt(0);
	sl0.upload();sl1.upload();sl0.set_amnt(0);
	l0.create_shadow(glm::vec3(0,0,0),50,50,5,4096);

	Material3D m0=Material3D(&r3d,40,16,0.25f);
	Material3D m1=Material3D(&r3d,1,64,2.0f);

	Bloom bloom=Bloom(&f);
	MSAA msaa=MSAA("shader/fbv_standard.shader","shader/fbf_standard.shader",
			16);
	FrameBuffer ifb=FrameBuffer("shader/fbv_standard.shader",
			"shader/fbf_standard.shader");

	Font fnt=Font("res/fonts/nimbus_roman.fnt","res/fonts/nimbus_roman.png",
			50,50);Text tft=Text(&fnt);
	tft.add("callidaria FRAMEWORK",glm::vec2(0,600));
	tft.add("this is a test",glm::vec2(0,550));
	tft.add("lorem Ipsum",glm::vec2(0,500));
	tft.add("AaBbCcDdEeFfGgHhIiJjKkLlMmNnOoPp",glm::vec2(0,450));
	tft.add("QqRrSsTtUuVvWwXxYyZz",glm::vec2(0,400));
	tft.load_wcam(&cam2d);tft.load_text();

	float pitch=0;float yaw=45.0f;int lfx,lfy;glm::mat4 ml=glm::mat4(1.0f);
	int flow_tex=0;
	bool run=true;while (run) {
		f.vsync(60);f.input(run);

		if (f.m.mcl) {
			pitch+=(f.m.my-lfy)*-0.1f;
			yaw+=(f.m.mx-lfx)*0.1f;
		} lfx=f.m.mx;lfy=f.m.my;
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
		if (f.kb.ka[SDLK_a])
			cam3d.pos-=0.05f*glm::normalize(glm::cross(
						cam3d.front,cam3d.up));
		else if (f.kb.ka[SDLK_d])
			cam3d.pos+=0.05f*glm::normalize(glm::cross(
						cam3d.front,cam3d.up));
		if (f.kb.ka[SDLK_r]) cam3d.pos+=0.05f*cam3d.up;
		else if (f.kb.ka[SDLK_f]) cam3d.pos-=0.05f*cam3d.up;

		l0.prepare_shadow();
		f.clear(0,0,0);

		r3d.prepare();glActiveTexture(GL_TEXTURE0);
		r3d.s3d.upload_matrix("proj",l0.proj);
		r3d.s3d.upload_matrix("view",l0.view);
		r3d.render_mesh(0,1);
		m0.upload();r3d.render_mesh(1,2);
		m1.upload();r3d.render_mesh(2,6);

		l0.close_shadow();
        
		msaa.bind();f.clear(0.1f,0.1f,0.1f);
		glCullFace(GL_BACK);r3d.prepare_wcam(&cam3d);
		r3d.s3d.upload_matrix("light_trans",l0.shadow_mat);
		l0.upload_shadow();

		r3d.render_mesh(0,1);
		m0.upload();r3d.render_mesh(1,2);
		m1.upload();r3d.render_mesh(2,6);

		msaa.blit(&ifb);msaa.close();bloom.bloom();f.clear(0,0,0);ifb.render();
		bloom.setup();f.clear(0,0,0);bloom.render();

		if (flow_tex>=138*3) { flow_tex=0; nw_sfx.play(); } else flow_tex++;
		f.clear_depth();tft.prepare();tft.render(flow_tex/3,glm::vec4(1,1,1,1));
        
		r2d.prepare();
		r2d.render_sprite(0,2);

		f.update();
	}

	bgm.remove();
	nw_sfx.remove();

	f.vanish();
	return 0;
}
