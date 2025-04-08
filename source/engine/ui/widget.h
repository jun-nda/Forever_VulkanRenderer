
#include "../renderer/renderer.h"

namespace engine
{

	class WidgetBase
	{

	public:
		explicit WidgetBase(const char* widgetName, const char* name);
		virtual ~WidgetBase() = default;

		void init() { onInit(); }
		void release() { onRelease(); }

		void tick(const engine::RuntimeModuleTickData& tickData, engine::VulkanContext* context);
		void tickWithCmd(const engine::RuntimeModuleTickData& tickData, VkCommandBuffer cmd, engine::VulkanContext* context);


	public:
		// event init.
		virtual void onInit() { }

		// event release.
		virtual void onRelease() {  }

	public:
		// Get widget name.
		const std::string& getName() const { return m_name; }
		const std::string& getWidgetName() const { return m_widgetName; }

		const UUID64u getRuntimeUUID() const { return m_runtimeUUID; }

		// Visible state set and get.
		void setVisible(bool bVisible) { m_bShow = bVisible; }
		bool getVisible() const { return m_bShow; }

	protected:
		UUID64u m_runtimeUUID;

		std::string m_widgetName;
		std::string m_name;

		// Cache renderer module.
		class RendererManager* m_renderer;

		// Cache engine handle.
		class Engine* m_engine;

		// Cache vulkan context.
		VulkanContext* m_context;

		// Widget show state.
		bool m_bShow;

		// Widget prev frame show state.
		bool m_bPrevShow;

		// Window show flags.
		ImGuiWindowFlags m_flags = 0;
	};
} 