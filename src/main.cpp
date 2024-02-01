#include <VeWindow.hpp>
#include <VePipeline.hpp>

int	main(void){
	VeWindow   lveWindow(WIDTH, HEIGHT);
	VePipeline	lvePipeline("shader/simpleShader.vert.spv", "shader/simpleShader.frag.spv");

	try {
		lveWindow.run();
	} catch(const std::exception& e) {
		std::cerr << e.what() << '\n';
		return (1);
	}
	return (0);
}
