#pragma once

#include "../definitions.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <GL/glew.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <AL/alc.h>
#include <cstdint>
#include <algorithm>
#include <string>
#include <optional>
#include <set>

struct Keyboard { bool ka[285] = { false }; };
struct Mouse { bool mcl=false,mcr=false;int mx,my,mxfr,myfr,mw; };
struct XBox { int xba[6];bool xbb[16] = { false }; };

struct FamilyCluster { std::optional<uint32_t> gfxFamily, frmFamily; };
struct SwapChainDetails {
	VkSurfaceCapabilitiesKHR cap;
	std::vector<VkSurfaceFormatKHR> formats;
	std::vector<VkPresentModeKHR> presentModes;
};

class Frame
{
public:
	Frame(const char* title,int screen,SDL_WindowFlags fs);
	Frame(const char* title="callidaria-program",int width=1280,int height=720,
			SDL_WindowFlags fs=(SDL_WindowFlags)0);
	Frame(const char* title,int screen,int width,int height,SDL_WindowFlags fs);

	void clear(float cx=0,float cy=0,float cz=0); 	// clear the window
	void update(); 					// update the window
	void update_framerelative();			// calculate current location of the mouse in ref to sdim
	void vsync(unsigned int frames); 		// cap frame count to specified value
	void input(bool &running); 			// check for user input
	void vanish(); 					// close program
private:
	void init();
	void setup(const char* title,int x,int y,int width,int height,SDL_WindowFlags fs);
	void get_screen(int screen,SDL_Rect* dim_screen);
#ifdef GFX_VULKAN
	// !!make the callback not so generic
	static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT ms,
			VkDebugUtilsMessageTypeFlagsEXT mt,const VkDebugUtilsMessengerCallbackDataEXT* cd,void* ud) {
		std::cerr << "vulkan error: " << cd->pMessage << '\n';
		return VK_FALSE;
	}
	void init_createInfo(VkDebugUtilsMessengerCreateInfoEXT &createInfo);
	FamilyCluster get_qFamily(VkPhysicalDevice in_gpu);
	SwapChainDetails get_qSwapChainSupport(VkPhysicalDevice gpu);
	std::vector<char> readShader(const std::string &path);
	VkShaderModule createShader(const std::vector<char> &source);
	void init_buffer(size_t bSize,VkBufferUsageFlags uFlags,VkMemoryPropertyFlags pFlags,
			VkDeviceMemory &bMemory,VkBuffer &buffer);
	void cpy_buffer(VkBuffer src,VkBuffer dst,size_t size);
#endif
public:
	Keyboard kb; Mouse mouse; std::vector<XBox> xb;		// input device
	bool event_active = false;				// for input case optimization
	int w_res, h_res;					// window dimensions
public: // !!change back to private
#ifdef GFX_OPENGL
	SDL_Window* m_frame; SDL_GLContext m_context;		// frame members
#endif
#ifdef GFX_VULKAN
	const int MAX_FRAMES_IN_FLIGHT = 2;
	GLFWwindow* m_frame; // ??maybe use glfw for opengl and vulkan application mult. advantages (valg.,macro red.)
	VkInstance vulkan;
	VkDebugUtilsMessengerEXT debugMessenger;
	VkPhysicalDevice m_gpu = VK_NULL_HANDLE;
	VkDevice m_logicalGPU;
	VkQueue m_gpuQueue,m_frmQueue;
	VkSurfaceKHR m_surface;
	VkExtent2D m_extent; // !!recycling the member variable
	VkSwapchainKHR m_swapchain;
	std::vector<VkImage> m_scImages;
	VkFormat m_scFormat;
	std::vector<VkImageView> m_scImageViews;
	VkRenderPass m_renderPass;
	VkPipelineLayout m_plLayout;
	VkPipeline m_pipeline;
	std::vector<VkFramebuffer> m_framebuffers;
	VkCommandPool m_commandPool;
	std::vector<VkCommandBuffer> m_commandBuffers;
	std::vector<VkSemaphore> m_rSemaphores,m_pSemaphores;
	std::vector<VkFence> m_fences,m_fFences;
	unsigned int m_currFrame = 0;
	VkBuffer vbo; // !!member prefix
	VkDeviceMemory m_vboMem;
#endif
	ALCdevice* m_alcdev; ALCcontext* m_alccon;			// audio members
	std::vector<SDL_GameController*> m_gc; SDL_Event m_fe;		// additional members
	unsigned int m_pT, m_cT, m_fps, m_tempFPS, m_lO;		// frame related members
};
