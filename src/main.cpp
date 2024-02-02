#include <VeDevice.hpp>
#include <VePipeline.hpp>
#include <VeWindow.hpp>

int	main(void){
	VeWindow	veWindow(WIDTH, HEIGHT);
	VeDevice	veDevice(veWindow);
	VePipeline	vePipeline(
		veDevice,
		"shader/simpleShader.vert.spv",
		"shader/simpleShader.frag.spv",
		VePipeline::defaultPipelineConfigInfo(WIDTH, HEIGHT)
	);

	try {
		veWindow.run();
	} catch(const std::exception& e) {
		std::cerr << e.what() << '\n';
		return (1);
	}
	return (0);
}
