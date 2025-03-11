
#include <engine.h>

class Editor
{
private:
	Editor() = default;

	bool init();
	bool release();

	//void tick(const engine::RuntimeModuleTickData& )

public:
	// Get editor instance
	static Editor* get( );

	// Editor run function
	int run(int argc, char** argv);


private:
	engine::GLFWWindows m_windows;
};