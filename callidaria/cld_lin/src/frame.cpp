#include "../frm/frame.h"

/*
 *	!!! ADD OUTPUT INFORMATION ABOUT SYSTEM !!!
 *	ALSO MAYBE IMPLEMENT WITH VIEWPORT RIGHT FROM THE BEGINNING ?
 * */
Frame::Frame(const char* title,int screen,SDL_WindowFlags fs)
{
	init();

	SDL_Rect dim_screen;
	get_screen(screen,&dim_screen);

	w_res = dim_screen.w;
	h_res = dim_screen.h;
	setup(title,dim_screen.x,dim_screen.y,dim_screen.w,dim_screen.h,fs);
}
Frame::Frame(const char* title,int width,int height,SDL_WindowFlags fs)
	: w_res(width),h_res(height)
{
	init();
	setup(title,SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED,width,height,fs);
}
Frame::Frame(const char* title,int screen,int width,int height,SDL_WindowFlags fs)
	: w_res(width),h_res(height)
{
	init();

	SDL_Rect dim_screen;
	get_screen(screen,&dim_screen);

	setup(title,dim_screen.x+100,dim_screen.y+100,width,height,fs);
}
void Frame::clear(float cx,float cy,float cz)
{
#ifdef GFX_OPENGL
	glClearColor(cx,cy,cz,1.0f);
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT); // ??maybe outside option to clear without depth buffer
#endif
#ifdef GFX_VULKAN
	vkWaitForFences(m_logicalGPU,1,&m_fences[m_currFrame],VK_TRUE,UINT64_MAX);
	unsigned int imgIndex;
	vkAcquireNextImageKHR(m_logicalGPU,m_swapchain,UINT64_MAX,m_rSemaphores[m_currFrame],
			VK_NULL_HANDLE,&imgIndex);
	if (m_fFences[imgIndex]!=VK_NULL_HANDLE)
		vkWaitForFences(m_logicalGPU,1,&m_fFences[imgIndex],VK_TRUE,UINT64_MAX);
	m_fFences[imgIndex] = m_fences[m_currFrame];

	VkSubmitInfo submitInfo{};
	submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
	VkSemaphore waitSemaphores[] = { m_rSemaphores[m_currFrame] };
	VkPipelineStageFlags waitStages[] = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };
	submitInfo.waitSemaphoreCount = 1;submitInfo.pWaitSemaphores = waitSemaphores;
	submitInfo.pWaitDstStageMask = waitStages;
	submitInfo.commandBufferCount = 1;submitInfo.pCommandBuffers = &m_commandBuffers[imgIndex];
	VkSemaphore signalSemaphores[] = { m_pSemaphores[m_currFrame] };
	submitInfo.signalSemaphoreCount = 1;submitInfo.pSignalSemaphores = signalSemaphores;
	vkResetFences(m_logicalGPU,1,&m_fences[m_currFrame]);
	if (vkQueueSubmit(m_gpuQueue,1,&submitInfo,m_fences[m_currFrame])!=VK_SUCCESS) printf("queue submit broke\n");

	VkPresentInfoKHR presentInfo{};
	presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
	presentInfo.waitSemaphoreCount = 1;presentInfo.pWaitSemaphores = signalSemaphores;
	VkSwapchainKHR swapChains[] = { m_swapchain };
	presentInfo.swapchainCount = 1;presentInfo.pSwapchains = swapChains;
	presentInfo.pImageIndices = &imgIndex;
	vkQueuePresentKHR(m_frmQueue,&presentInfo);
	m_currFrame = (m_currFrame+1)%MAX_FRAMES_IN_FLIGHT;
#endif
}
#ifdef SDL_OPENGL
void Frame::update() { SDL_GL_SwapWindow(m_frame); }
#endif
/*
 *	THIS MIGHT BE EFFICTIVE FOR A VSYNC FUNCTION BUT THIS IS !!!NOT!!! APPROPRIATE FOR DELTA TIME RELIANCE !
 *	MAKE VSYNC OPTIONAL AND BASE TIME RELATED UPDATES AND PHYSICS TO A LEGITIMATE DELTA TIME .
 *	THIS ISN'T THE 90s ANYMORE GODDAMMIT . THE LATER THIS HAPPENS THE MORE CODE HAS TO BE CHANGED WHEN IT DOES
 * */
void Frame::vsync(unsigned int mf)
{
	m_pT = m_cT;
	m_cT = SDL_GetTicks();
	m_tempFPS++;
	if (m_cT-m_lO>=1000) {
		m_lO = m_cT;
		m_fps = m_tempFPS;
		m_tempFPS = 0;
	} if (m_cT-m_pT<1000/mf) SDL_Delay(1000/mf-SDL_GetTicks()+m_pT);
}
void Frame::input(bool &running)
{
#ifdef GFX_OPENGL
	event_active = false;
	while (SDL_PollEvent(&m_fe)) {
		event_active = true;
		running = m_fe.type!=SDL_QUIT; // exit the program when closing is requested

		// read keyboard input
		// ??are scancodes slower that sdlks
		if (m_fe.type==SDL_KEYDOWN) kb.ka[m_fe.key.keysym.scancode] = true;
		if (m_fe.type==SDL_KEYUP) kb.ka[m_fe.key.keysym.scancode] = false;

		// read text input
		// ??better performance than check if pressed
		/*for (int i=0;i<285;i++) {
			
		}*/

		// read mouse input
		SDL_GetMouseState(&mouse.mx,&mouse.my);
		mouse.mxfr = ((float)mouse.mx/w_res)*1920.0f; // ??make those optional
		mouse.myfr = ((float)(h_res-mouse.my)/h_res)*1080.0f;
		// !!fix on move cancellation && carry boolean when button released
		if (m_fe.type==SDL_MOUSEBUTTONDOWN) { // ??breakdown to boolean equasion
			mouse.mcl = m_fe.button.button==SDL_BUTTON_LEFT;
			mouse.mcr = m_fe.button.button==SDL_BUTTON_RIGHT;
		} if (m_fe.type==SDL_MOUSEBUTTONUP) { // !!not the practical way
			mouse.mcl = m_fe.button.button!=SDL_BUTTON_LEFT;
			mouse.mcr = m_fe.button.button!=SDL_BUTTON_RIGHT;
		}
		/*mouse.mcl = (m_fe.type==SDL_MOUSEBUTTONDOWN&&m_fe.button.button==SDL_BUTTON_LEFT)
				||(m_fe.type==SDL_MOUSEBUTTONUP&&m_fe.button.button!=SDL_BUTTON_LEFT);
		mouse.mcr = (m_fe.type==SDL_MOUSEBUTTONDOWN&&m_fe.button.button==SDL_BUTTON_RIGHT)
				||(m_fe.type==SDL_MOUSEBUTTONUP&&m_fe.button.button!=SDL_BUTTON_RIGHT);*/
		/*mouse.mcl = m_fe.button.button==SDL_BUTTON_LEFT&&m_fe.type==SDL_MOUSEBUTTONDOWN;
		mouse.mcr = m_fe.button.button==SDL_BUTTON_RIGHT&&m_fe.type==SDL_MOUSEBUTTONDOWN;*/
		mouse.mw = m_fe.wheel.y;

		// read controller input
		for (int i=0;i<m_gc.size();i++) {
			for (int j=0;j<6;j++)
				xb.at(i).xba[j] = SDL_GameControllerGetAxis(m_gc.at(i),(SDL_GameControllerAxis)j);
			for (int j=0;j<16;j++)
				xb.at(i).xbb[j] = SDL_GameControllerGetButton(m_gc.at(i),(SDL_GameControllerButton)j);
		}
		// face buttons have the default xbox layout so for sony it is X=A,O=B,sq=X and delta=Y
		// results in SDL_CONTROLLER_BUTTON_* macro for nintendo controllers having exchanged a&b recognition
	}
#endif
#ifdef GFX_VULKAN
	glfwPollEvents();
	running = !glfwWindowShouldClose(m_frame);
#endif
}
void Frame::vanish()
{
	// ??doing this with an array reference even cleaner when converted & test when valgrind isn't mad anymore
	for (int i=0;i<m_gc.size();i++) SDL_GameControllerClose(m_gc.at(i)); // closing controller reference

	// closing audio context & device
	alcMakeContextCurrent(NULL);
	alcDestroyContext(m_alccon);
	alcCloseDevice(m_alcdev);

	// closing render context & program
#ifdef GFX_OPENGL
	SDL_GL_DeleteContext(m_context);
	SDL_Quit();
#endif
#ifdef GFX_VULKAN
	vkDeviceWaitIdle(m_logicalGPU);
	for (int i=0;i<MAX_FRAMES_IN_FLIGHT;i++) {
		vkDestroySemaphore(m_logicalGPU,m_rSemaphores[i],nullptr);
		vkDestroySemaphore(m_logicalGPU,m_pSemaphores[i],nullptr);
		vkDestroyFence(m_logicalGPU,m_fences[i],nullptr);
	}
	vkDestroyCommandPool(m_logicalGPU,m_commandPool,nullptr);
	for (VkFramebuffer fb : m_framebuffers) vkDestroyFramebuffer(m_logicalGPU,fb,nullptr);
	vkDestroyPipeline(m_logicalGPU,m_pipeline,nullptr);
	vkDestroyPipelineLayout(m_logicalGPU,m_plLayout,nullptr);
	vkDestroyRenderPass(m_logicalGPU,m_renderPass,nullptr);
	for (VkImageView iv : m_scImageViews) vkDestroyImageView(m_logicalGPU,iv,nullptr);
	vkDestroySwapchainKHR(m_logicalGPU,m_swapchain,nullptr);
	vkDestroyBuffer(m_logicalGPU,vbo,nullptr);
	vkFreeMemory(m_logicalGPU,m_vboMem,nullptr);
	vkDestroyDevice(m_logicalGPU,nullptr);
	auto func = (PFN_vkDestroyDebugUtilsMessengerEXT)
		vkGetInstanceProcAddr(vulkan,"vkDestroyDebugUtilsMessengerEXT");
	func(vulkan,debugMessenger,nullptr);
	vkDestroySurfaceKHR(vulkan,m_surface,nullptr);
	vkDestroyInstance(vulkan,nullptr);
	
	glfwDestroyWindow(m_frame);
	glfwTerminate();
#endif
}
void Frame::init()
{
	const char* abs = "non selected abstraction";
	const char* os = "non approved version";
#ifdef __linux__
	os = "linux version";
#endif
#ifdef _WIN32
	os = "windows version";
#endif
#ifdef __APPLE__
	os = "so you decided to use a mac... *sigh* - The Cherno\n"
		+"i have not enough money and patience to port this to mac. so proceed at your own risk\n";
#endif
#ifdef GFX_OPENGL
	abs = "OpenGL";
#endif
#ifdef GFX_VULKAN
	abs = "Vulkan";
#endif
	printf("starting callidaria... %s running with %s\n",os,abs);
#ifdef GFX_OPENGL
	// sdl setup
	SDL_Init(SDL_INIT_EVERYTHING);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK,SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION,3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION,3);
	SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE,8);
#endif
#ifdef GFX_VULKAN
	glfwInit();
	glfwWindowHint(GLFW_CLIENT_API,GLFW_NO_API);
	glfwWindowHint(GLFW_RESIZABLE,GLFW_FALSE);
#endif
}
void Frame::setup(const char* title,int x,int y,int width,int height,SDL_WindowFlags fs)
{
#ifdef GFX_OPENGL
	// creating window
	m_frame = SDL_CreateWindow(title,x,y,width,height,SDL_WINDOW_OPENGL);
	SDL_SetWindowFullscreen(m_frame,fs);
	m_context = SDL_GL_CreateContext(m_frame);

	// opengl setup
	glewInit();
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);

	glViewport(0,0,width,height); // ??any impact
#endif
#ifdef GFX_VULKAN
	// creating window
	m_frame = glfwCreateWindow(width,height,title,0,nullptr);

	// vulkan setup
	VkApplicationInfo appInfo{}; // ??which is just dressing which is actually important
	appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
	appInfo.pApplicationName = "callidaria vulkan";
	appInfo.applicationVersion = VK_MAKE_VERSION(1,0,0);
	appInfo.apiVersion = VK_API_VERSION_1_0;

	VkInstanceCreateInfo createInfo{};
	createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
	createInfo.pApplicationInfo = &appInfo;

	unsigned int glfwExtensionCount = 0;
	const char** glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);
	std::vector<const char*> extensions(glfwExtensions,glfwExtensions+glfwExtensionCount); // ??rewrite
	extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
	createInfo.enabledExtensionCount = static_cast<unsigned int>(extensions.size());
	createInfo.ppEnabledExtensionNames = extensions.data(); // !!this is much easier to write that portrayed

	// validation layers
	VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo;
	const std::vector<const char*> vl = { "VK_LAYER_KHRONOS_validation" }; // ??stringset
	const std::vector<const char*> de = { VK_KHR_SWAPCHAIN_EXTENSION_NAME };
	createInfo.enabledLayerCount = static_cast<unsigned int>(vl.size());
	createInfo.ppEnabledLayerNames = vl.data();
	init_createInfo(debugCreateInfo);
	createInfo.pNext = (VkDebugUtilsMessengerCreateInfoEXT*)&debugCreateInfo;
	vkCreateInstance(&createInfo,nullptr,&vulkan); // instance definition

	// debug messenger
	VkDebugUtilsMessengerCreateInfoEXT createNewInfo;
	init_createInfo(createNewInfo); // !!analyzing naming and pipeline
	auto func = (PFN_vkCreateDebugUtilsMessengerEXT)
		vkGetInstanceProcAddr(vulkan,"vkCreateDebugUtilsMessengerEXT"); // ??stringset
	func(vulkan,&createNewInfo,nullptr,&debugMessenger); // !!kill branch

	glfwCreateWindowSurface(vulkan,m_frame,nullptr,&m_surface); // surface

	// physical device
	unsigned int deviceCount = 0;
	vkEnumeratePhysicalDevices(vulkan,&deviceCount,nullptr); // !!this is BULLSHIT *kick*
	std::vector<VkPhysicalDevice> gpus(deviceCount);
	vkEnumeratePhysicalDevices(vulkan,&deviceCount,gpus.data());
	for (const auto &gpu : gpus) {
		FamilyCluster qfi = get_qFamily(gpu);
		
		bool extSupport;
		unsigned int extCount;
		vkEnumerateDeviceExtensionProperties(gpu,nullptr,&extCount,nullptr); // ??repetitive call
		std::vector<VkExtensionProperties> aExtensions(extCount);
		vkEnumerateDeviceExtensionProperties(gpu,nullptr,&extCount,aExtensions.data());
		std::set<std::string> rExtensions(de.begin(),de.end()); // !!change to const char*
		for (const auto &ext : aExtensions) rExtensions.erase(ext.extensionName); // !!anti performance
		extSupport = rExtensions.empty();

		bool swapAdequate = false;
		if (extSupport) { // ??self minimizing boolean for swapAdequate always false if extSupport is
			SwapChainDetails scd_rd = get_qSwapChainSupport(gpu);
			swapAdequate = !scd_rd.formats.empty()&&!scd_rd.presentModes.empty();
		} if (qfi.gfxFamily.has_value()&&qfi.frmFamily.has_value()&&extSupport
				&&swapAdequate) { m_gpu = gpu; break; }
		// !!write gpu priority verification
	}

	// logical device
	FamilyCluster gpu_fi = get_qFamily(m_gpu); // ??double usage not performant. new solution
	std::vector<VkDeviceQueueCreateInfo> dqcis;
	std::set<uint32_t> uqf = { gpu_fi.gfxFamily.value(),gpu_fi.frmFamily.value() };
	float qpriority = 1.0f;
	for (uint32_t qf : uqf) {
		VkDeviceQueueCreateInfo dqci{};
		dqci.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
		dqci.queueFamilyIndex = qf;
		dqci.queueCount = 1;
		dqci.pQueuePriorities = &qpriority;
		dqcis.push_back(dqci);
	} VkPhysicalDeviceFeatures dfeature{};

	VkDeviceCreateInfo deviceInfo{}; // ??device info creation method
	deviceInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
	deviceInfo.pQueueCreateInfos = dqcis.data();
	deviceInfo.queueCreateInfoCount = static_cast<unsigned int>(dqcis.size());
	deviceInfo.pEnabledFeatures = &dfeature;
	deviceInfo.enabledExtensionCount = static_cast<unsigned int>(de.size());
	deviceInfo.enabledLayerCount = static_cast<unsigned int>(vl.size());
	deviceInfo.ppEnabledLayerNames = vl.data();
	deviceInfo.ppEnabledExtensionNames = de.data();
	vkCreateDevice(m_gpu,&deviceInfo,nullptr,&m_logicalGPU);
	vkGetDeviceQueue(m_logicalGPU,gpu_fi.gfxFamily.value(),0,&m_gpuQueue);
	vkGetDeviceQueue(m_logicalGPU,gpu_fi.frmFamily.value(),0,&m_frmQueue);

	// swap chain
	SwapChainDetails scd = get_qSwapChainSupport(m_gpu);
	VkSurfaceFormatKHR sFormat;
	bool set=false;for (const auto &format : scd.formats) {
		if (format.format==VK_FORMAT_B8G8R8A8_SRGB&&format.colorSpace==VK_COLOR_SPACE_SRGB_NONLINEAR_KHR) {
			printf("non-linear srgb mode supported and set\n");
			sFormat = format;
			set = true; break;
		}
	} if (!set) sFormat = scd.formats[0]; // !!implement better choosing system
	VkPresentModeKHR pMode;
	set=false;for (const auto &presentMode : scd.presentModes) {
		if (presentMode==VK_PRESENT_MODE_MAILBOX_KHR) {
			printf("triple buffering supported and set\n");
			pMode = presentMode;
			set = true; break;
		}
	} if (!set) pMode = VK_PRESENT_MODE_FIFO_KHR;
	if (scd.cap.currentExtent.width!=UINT32_MAX) m_extent = scd.cap.currentExtent;
	else {
		int width,height;
		glfwGetFramebufferSize(m_frame,&width,&height);
		VkExtent2D aExtent = { static_cast<unsigned int>(width),static_cast<unsigned int>(height) };
		aExtent.width = std::max(scd.cap.minImageExtent.width,
				std::min(scd.cap.maxImageExtent.width,aExtent.width));
		aExtent.height = std::max(scd.cap.minImageExtent.height,
				std::min(scd.cap.maxImageExtent.height,aExtent.height));
		m_extent = aExtent;
	} unsigned int iCount = scd.cap.minImageCount+1;
	if (scd.cap.maxImageCount>0&&iCount>scd.cap.maxImageCount) iCount = scd.cap.maxImageCount;

	VkSwapchainCreateInfoKHR swapInfo{};
	swapInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
	swapInfo.surface = m_surface;
	swapInfo.minImageCount = iCount;
	swapInfo.imageFormat = sFormat.format;
	swapInfo.imageColorSpace = sFormat.colorSpace;
	swapInfo.imageExtent = m_extent;
	swapInfo.imageArrayLayers = 1;
	swapInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
	unsigned int qFamilyIndices[] = { gpu_fi.gfxFamily.value(),gpu_fi.frmFamily.value() }; // !!direct cast
	// !!reusing gpu_fi
	if (gpu_fi.gfxFamily!=gpu_fi.frmFamily) {
		swapInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
		swapInfo.queueFamilyIndexCount = 2;
		swapInfo.pQueueFamilyIndices = qFamilyIndices;
	} else swapInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
	swapInfo.preTransform = scd.cap.currentTransform;
	swapInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
	swapInfo.presentMode = pMode;
	swapInfo.clipped = VK_TRUE;
	swapInfo.oldSwapchain = VK_NULL_HANDLE;
	if (vkCreateSwapchainKHR(m_logicalGPU,&swapInfo,nullptr,&m_swapchain)!=VK_SUCCESS)
		printf("swapchain broke\n");

	vkGetSwapchainImagesKHR(m_logicalGPU,m_swapchain,&iCount,nullptr);
	m_scImages.resize(iCount);
	vkGetSwapchainImagesKHR(m_logicalGPU,m_swapchain,&iCount,m_scImages.data());
	m_scFormat = sFormat.format;

	// image view
	m_scImageViews.resize(m_scImages.size()); // !!mother father gentolmain
	for (int i=0;i<m_scImages.size();i++) { // ??presave size value of m_scImages
		VkImageViewCreateInfo iViewInfo{};
		iViewInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
		iViewInfo.image = m_scImages[i];
		iViewInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
		iViewInfo.format = sFormat.format;
		iViewInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
		iViewInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
		iViewInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
		iViewInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;
		iViewInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		iViewInfo.subresourceRange.baseMipLevel = 0;
		iViewInfo.subresourceRange.levelCount = 1;
		iViewInfo.subresourceRange.baseArrayLayer = 0;
		iViewInfo.subresourceRange.layerCount = 1;
		if (vkCreateImageView(m_logicalGPU,&iViewInfo,nullptr,&m_scImageViews[i])!=VK_SUCCESS)
			printf("imageView at index %i broke\n",i);
	}

	// render pass
	VkAttachmentDescription attDesc{};
	attDesc.format = sFormat.format;
	attDesc.samples = VK_SAMPLE_COUNT_1_BIT;
	attDesc.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
	attDesc.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
	attDesc.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
	attDesc.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
	attDesc.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
	attDesc.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;
	VkAttachmentReference attRef{};
	attRef.attachment = 0;
	attRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
	VkSubpassDescription subDesc{};
	subDesc.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
	subDesc.colorAttachmentCount = 1;
	subDesc.pColorAttachments = &attRef;
	VkSubpassDependency dep{};
	dep.srcSubpass = VK_SUBPASS_EXTERNAL;
	dep.dstSubpass = 0;
	dep.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
	dep.srcAccessMask = 0;
	dep.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
	dep.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;

	VkRenderPassCreateInfo rPassInfo{};
	rPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
	rPassInfo.attachmentCount = 1;rPassInfo.pAttachments = &attDesc;
	rPassInfo.subpassCount = 1;rPassInfo.pSubpasses = &subDesc;
	rPassInfo.dependencyCount = 1;rPassInfo.pDependencies = &dep;
	if (vkCreateRenderPass(m_logicalGPU,&rPassInfo,nullptr,&m_renderPass)!=VK_SUCCESS)
		printf("render pass broke");

	// pipeline
	std::vector<char> vertexShaderSource = readShader("vk_shader/vertex.spv");
	std::vector<char> fragmentShaderSource = readShader("vk_shader/fragment.spv");
	VkShaderModule vertexShader = createShader(vertexShaderSource);
	VkShaderModule fragmentShader = createShader(fragmentShaderSource);

	VkPipelineShaderStageCreateInfo vStageInfo{};
	vStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
	vStageInfo.stage = VK_SHADER_STAGE_VERTEX_BIT;
	vStageInfo.module = vertexShader;
	vStageInfo.pName = "main";
	VkPipelineShaderStageCreateInfo fStageInfo{};
	fStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
	fStageInfo.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
	fStageInfo.module = fragmentShader;
	fStageInfo.pName = "main";
	VkPipelineShaderStageCreateInfo shaderStages[] = { vStageInfo,fStageInfo };

	const float vertices[] = { // §§test values
		0.0f,-0.5f,1.0f,0.0f,0.0f,
		0.5f,0.5f,0.0f,1.0f,0.0f,
		-0.5f,0.5f,0.0f,0.0f,1.0f
	}; VkVertexInputBindingDescription bindDesc{};
	bindDesc.binding = 0;
	bindDesc.stride = 5*sizeof(float);
	bindDesc.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
	std::array<VkVertexInputAttributeDescription,2> attribDesc{};
	attribDesc[0].binding = 0;
	attribDesc[0].location = 0;
	attribDesc[0].format = VK_FORMAT_R32G32_SFLOAT;
	attribDesc[0].offset = 0;
	attribDesc[1].binding = 0;
	attribDesc[1].location = 1;
	attribDesc[1].format = VK_FORMAT_R32G32B32_SFLOAT;
	attribDesc[1].offset = 2*sizeof(float);
	VkPipelineVertexInputStateCreateInfo inputInfo{};
	inputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
	inputInfo.vertexBindingDescriptionCount = 1;
	inputInfo.vertexAttributeDescriptionCount = static_cast<unsigned int>(attribDesc.size()); // §§break
	inputInfo.pVertexBindingDescriptions = &bindDesc;
	inputInfo.pVertexAttributeDescriptions = attribDesc.data();

	VkPipelineInputAssemblyStateCreateInfo assemblyInfo{};
	assemblyInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
	assemblyInfo.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
	assemblyInfo.primitiveRestartEnable = VK_FALSE;

	VkViewport viewport{};
	viewport.x = 0.0f;viewport.y = 0.0f;
	viewport.width = (float)m_extent.width;viewport.height = (float)m_extent.height;
	viewport.minDepth = 0.0f;viewport.maxDepth = 1.0f;
	VkRect2D scissor{};
	scissor.offset = { 0,0 };
	scissor.extent = m_extent;
	VkPipelineViewportStateCreateInfo vsInfo{};
	vsInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
	vsInfo.viewportCount = 1;vsInfo.pViewports = &viewport;
	vsInfo.scissorCount = 1;vsInfo.pScissors = &scissor;

	VkPipelineRasterizationStateCreateInfo rasterInfo{};
	rasterInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
	rasterInfo.depthClampEnable = VK_FALSE;
	rasterInfo.rasterizerDiscardEnable = VK_FALSE;
	rasterInfo.polygonMode = VK_POLYGON_MODE_FILL;
	rasterInfo.lineWidth = 1.0f;
	rasterInfo.cullMode = VK_CULL_MODE_BACK_BIT;
	rasterInfo.frontFace = VK_FRONT_FACE_CLOCKWISE;
	rasterInfo.depthBiasEnable = VK_FALSE;
	VkPipelineMultisampleStateCreateInfo msaaInfo{};
	msaaInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
	msaaInfo.sampleShadingEnable = VK_FALSE;
	msaaInfo.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;

	VkPipelineColorBlendAttachmentState colourBlend{};
	colourBlend.colorWriteMask = VK_COLOR_COMPONENT_R_BIT|VK_COLOR_COMPONENT_G_BIT|VK_COLOR_COMPONENT_B_BIT
		|VK_COLOR_COMPONENT_A_BIT;
	colourBlend.blendEnable = VK_FALSE;
	VkPipelineColorBlendStateCreateInfo blendInfo{};
	blendInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
	blendInfo.logicOpEnable = VK_FALSE;
	blendInfo.attachmentCount = 1;
	blendInfo.pAttachments = &colourBlend;

	// ??add dynamic state

	VkPipelineLayoutCreateInfo pLayoutInfo{};
	pLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
	if (vkCreatePipelineLayout(m_logicalGPU,&pLayoutInfo,nullptr,&m_plLayout)) printf("pipeline broke\n");
	VkGraphicsPipelineCreateInfo pipelineInfo{};
	pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
	pipelineInfo.stageCount = 2;pipelineInfo.pStages = shaderStages;
	pipelineInfo.pVertexInputState = &inputInfo;
	pipelineInfo.pInputAssemblyState = &assemblyInfo;
	pipelineInfo.pViewportState = &vsInfo;
	pipelineInfo.pRasterizationState = &rasterInfo;
	pipelineInfo.pMultisampleState = &msaaInfo;
	pipelineInfo.pColorBlendState = &blendInfo;
	pipelineInfo.layout = m_plLayout;
	pipelineInfo.renderPass = m_renderPass;
	pipelineInfo.subpass = 0;
	if (vkCreateGraphicsPipelines(m_logicalGPU,VK_NULL_HANDLE,1,&pipelineInfo,nullptr,&m_pipeline)!=VK_SUCCESS)
		printf("gfx pipeline broke\n");

	vkDestroyShaderModule(m_logicalGPU,vertexShader,nullptr);
	vkDestroyShaderModule(m_logicalGPU,fragmentShader,nullptr);

	// framebuffers
	m_framebuffers.resize(m_scImageViews.size());
	for (int i=0;i<m_scImageViews.size();i++) {
		VkImageView proc[] = { m_scImageViews[i] };
		VkFramebufferCreateInfo fbInfo{};
		fbInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
		fbInfo.renderPass = m_renderPass;
		fbInfo.attachmentCount = 1;fbInfo.pAttachments = proc;
		fbInfo.width = m_extent.width;fbInfo.height = m_extent.height;
		fbInfo.layers = 1;
		if (vkCreateFramebuffer(m_logicalGPU,&fbInfo,nullptr,&m_framebuffers[i])!=VK_SUCCESS)
			printf("framebuffer creation broke at index %i\n",i);
	}

	// command pool & buffers
	VkCommandPoolCreateInfo poolInfo{};
	poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
	poolInfo.queueFamilyIndex = gpu_fi.gfxFamily.value();
	if (vkCreateCommandPool(m_logicalGPU,&poolInfo,nullptr,&m_commandPool)!=VK_SUCCESS) printf("pool broke\n");

	VkBufferCreateInfo bufferInfo{};
	bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
	bufferInfo.size = sizeof(vertices[0])*15;
	bufferInfo.usage = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;
	bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
	if (vkCreateBuffer(m_logicalGPU,&bufferInfo,nullptr,&vbo)!=VK_SUCCESS) printf("vbo broke\n");
	VkMemoryRequirements memReq;
	vkGetBufferMemoryRequirements(m_logicalGPU,vbo,&memReq);
	VkPhysicalDeviceMemoryProperties memProp;
	vkGetPhysicalDeviceMemoryProperties(m_gpu,&memProp);
	VkMemoryPropertyFlags props = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT|VK_MEMORY_PROPERTY_HOST_COHERENT_BIT;
	uint32_t memType;for (int i=0;i<memProp.memoryTypeCount;i++) {
		if (memReq.memoryTypeBits&(1<<i)
			&&(memProp.memoryTypes[i].propertyFlags&props)==props) { memType=i;break; }
	} VkMemoryAllocateInfo mallocInfo{};
	mallocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
	mallocInfo.allocationSize = memReq.size;
	mallocInfo.memoryTypeIndex = memType;
	if (vkAllocateMemory(m_logicalGPU,&mallocInfo,nullptr,&m_vboMem)!=VK_SUCCESS) printf("vbo mem broke\n");
	vkBindBufferMemory(m_logicalGPU,vbo,m_vboMem,0);
	void* vram;
	vkMapMemory(m_logicalGPU,m_vboMem,0,bufferInfo.size,0,&vram);
	memcpy(vram,&vertices,bufferInfo.size);
	vkUnmapMemory(m_logicalGPU,m_vboMem);

	m_commandBuffers.resize(m_framebuffers.size());
	VkCommandBufferAllocateInfo cBufferInfo{};
	cBufferInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
	cBufferInfo.commandPool = m_commandPool;
	cBufferInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
	cBufferInfo.commandBufferCount = (unsigned int) m_commandBuffers.size();
	if (vkAllocateCommandBuffers(m_logicalGPU,&cBufferInfo,m_commandBuffers.data())!=VK_SUCCESS)
		printf("command buffers broke\n");

	VkBuffer vbs[] = { vbo };
	VkDeviceSize offsets[] = { 0 };
	for (int i=0;i<m_commandBuffers.size();i++) {
		VkCommandBufferBeginInfo commandInfo{};
		commandInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
		if (vkBeginCommandBuffer(m_commandBuffers[i],&commandInfo)!=VK_SUCCESS)
			printf("begin buffers broke\n");
		VkRenderPassBeginInfo rPassInfo{};
		rPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
		rPassInfo.renderPass = m_renderPass;
		rPassInfo.framebuffer = m_framebuffers[i];
		rPassInfo.renderArea.offset = { 0,0 };
		rPassInfo.renderArea.extent = m_extent;
		rPassInfo.clearValueCount = 1;
		VkClearValue clearColour = { 0.0f,0.0f,0.0f,1.0f };
		rPassInfo.pClearValues = &clearColour;
		vkCmdBeginRenderPass(m_commandBuffers[i],&rPassInfo,VK_SUBPASS_CONTENTS_INLINE);
		vkCmdBindPipeline(m_commandBuffers[i],VK_PIPELINE_BIND_POINT_GRAPHICS,m_pipeline);
		vkCmdBindVertexBuffers(m_commandBuffers[i],0,1,vbs,offsets);
		vkCmdDraw(m_commandBuffers[i],3,1,0,0);
		vkCmdEndRenderPass(m_commandBuffers[i]);
		if (vkEndCommandBuffer(m_commandBuffers[i])!=VK_SUCCESS) printf("ending command buffer broke\n");
	}

	// synchronisation
	m_rSemaphores.resize(MAX_FRAMES_IN_FLIGHT);m_pSemaphores.resize(MAX_FRAMES_IN_FLIGHT);
	m_fences.resize(MAX_FRAMES_IN_FLIGHT);m_fFences.resize(m_scImages.size(),VK_NULL_HANDLE);
	VkSemaphoreCreateInfo semaphoreInfo{};
	semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
	VkFenceCreateInfo fenceInfo{};
	fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
	fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;
	for (int i=0;i<MAX_FRAMES_IN_FLIGHT;i++) {
		if (vkCreateSemaphore(m_logicalGPU,&semaphoreInfo,nullptr,&m_rSemaphores[i])!=VK_SUCCESS
			||vkCreateSemaphore(m_logicalGPU,&semaphoreInfo,nullptr,&m_pSemaphores[i])!=VK_SUCCESS
			||vkCreateFence(m_logicalGPU,&fenceInfo,nullptr,&m_fences[i])!=VK_SUCCESS)
				printf("syncronization broke\n");
	}

	// !!add swap chain recreation
#endif

	// openal setup
	m_alcdev = alcOpenDevice(NULL);
	m_alccon = alcCreateContext(m_alcdev,NULL);
	alcMakeContextCurrent(m_alccon);

	// controller setup
	int gcc = 0;
	while (SDL_IsGameController(gcc)) {
		m_gc.push_back(SDL_GameControllerOpen(gcc));
		xb.push_back(XBox()); // !!negative points for style ...maybe stack usage instead???
		gcc++;
	} m_cT = 0; m_fps = 0; m_tempFPS = 0; m_lO = 0; // ??all necessary & syntax
}
void Frame::get_screen(int screen,SDL_Rect* dim_screen)
{
	if (screen<SDL_GetNumVideoDisplays()&&SDL_GetDisplayBounds(screen,dim_screen)==0)
		printf("\033[1;36mmaximum resolution of selected screen is: %ix%i\n",dim_screen->w,dim_screen->h);
	else {
		printf("\033[1;31mscreen could not be set: %s\n",SDL_GetError());
		printf("\033[1;36m\t=> falling back to standard configuration\033[0m\n");
		dim_screen->x = 0; dim_screen->y = 0; dim_screen->w = 1280; dim_screen->h = 720;
	}
}
#ifdef GFX_VULKAN
void Frame::init_createInfo(VkDebugUtilsMessengerCreateInfoEXT &createInfo) // ??definition flawed
{
	createInfo = {};
	createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
	createInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT
		|VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT|VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
	createInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT
		|VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT|VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
	createInfo.pfnUserCallback = debugCallback;
}
FamilyCluster Frame::get_qFamily(VkPhysicalDevice in_gpu)
{
	FamilyCluster out;
	unsigned int qfc = 0;
	vkGetPhysicalDeviceQueueFamilyProperties(in_gpu,&qfc,nullptr);
	std::vector<VkQueueFamilyProperties> qfs(qfc);
	vkGetPhysicalDeviceQueueFamilyProperties(in_gpu,&qfc,qfs.data());
	int i=0;for (const auto &qf : qfs) {
		if (qf.queueFlags&VK_QUEUE_GRAPHICS_BIT) out.gfxFamily = i; // ??logic for value shift
		VkBool32 frmSupport = false;
		vkGetPhysicalDeviceSurfaceSupportKHR(in_gpu,i,m_surface,&frmSupport);
		if (frmSupport) out.frmFamily = i;
		if (out.gfxFamily.has_value()&&out.frmFamily.has_value()) break;
		i++; // ??reprioritized gpu order
	} return out;
}
SwapChainDetails Frame::get_qSwapChainSupport(VkPhysicalDevice gpu)
{
	SwapChainDetails out;
	vkGetPhysicalDeviceSurfaceCapabilitiesKHR(gpu,m_surface,&out.cap);
	unsigned int fCount;
	vkGetPhysicalDeviceSurfaceFormatsKHR(gpu,m_surface,&fCount,nullptr); // !!break down multiple calls
	if (fCount!=0) {
		out.formats.resize(fCount); // !!verbose and gentleman way of doing this. not really my style
		vkGetPhysicalDeviceSurfaceFormatsKHR(gpu,m_surface,&fCount,out.formats.data());
	} unsigned int pModeCount;
	vkGetPhysicalDeviceSurfacePresentModesKHR(gpu,m_surface,&pModeCount,nullptr); // !!again, break down
	if (pModeCount!=0) {
		out.presentModes.resize(pModeCount);
		vkGetPhysicalDeviceSurfacePresentModesKHR(gpu,m_surface,&pModeCount,out.presentModes.data());
	} return out;
}
std::vector<char> Frame::readShader(const std::string &path)
{
	std::ifstream file(path,std::ios::ate|std::ios::binary);
	if (!file.is_open()) printf("shader %s not found\n",path.c_str());
	size_t fsize = (size_t)file.tellg();
	std::vector<char> out(fsize);
	file.seekg(0);file.read(out.data(),fsize);file.close();
	return out;
}
VkShaderModule Frame::createShader(const std::vector<char> &source)
{
	VkShaderModuleCreateInfo shaderInfo{};
	shaderInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
	shaderInfo.codeSize = source.size();
	shaderInfo.pCode = reinterpret_cast<const uint32_t*>(source.data());
	VkShaderModule out;
	if (vkCreateShaderModule(m_logicalGPU,&shaderInfo,nullptr,&out)!=VK_SUCCESS) printf("shader broke\n");
	return out;
}
#endif
