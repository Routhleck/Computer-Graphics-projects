#include "Gui.h"

void renderGUI() {
	// 重置 OpenGL 渲染状态
	glUseProgram(0);
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);

	// 开始新的 ImGui 帧
	ImGui_ImplOpenGL2_NewFrame();
	ImGui_ImplGLUT_NewFrame();
	ImGui::NewFrame();

	// 设置界面尺寸
	ImGuiIO& io = ImGui::GetIO();
	io.DisplaySize = ImVec2(static_cast<float>(window_width), static_cast<float>(window_height));

	// 创建和渲染 GUI 界面的代码...
	if (ImGui::Begin("Setting", nullptr, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_AlwaysAutoResize)) {
		// 添加 "Continue" 按钮
		if (ImGui::Button("Continue")) {
			// 处理点击 "Continue" 按钮的逻辑
			isCursor = false;
		}

		// 添加类型部分
		if (ImGui::CollapsingHeader("Camera parameter")) {
			// 添加 摄像机 对应的参数控件
			ImGui::SliderFloat("Sensitivity", &sensitivity, 0.1f, 1.0f);
			ImGui::SliderFloat("Camera Speed", &cameraSpeed, 0.01f, 0.50f);
			ImGui::SliderFloat("FOV", &fov, 15.0f, 120.0f);
			// 添加更多的参数控件...
		}

		if (ImGui::CollapsingHeader("Snow Particles")) {
			// 添加 粒子 对应的参数控件
			ImGui::Checkbox("Alive", &particles_alive);
			ImGui::SliderInt("Max particles", &MAX_PARTICLES, 0, 5000);
			ImGui::SliderFloat("Life", &particles_life, 0.0f, 10.0f);
			ImGui::SliderInt("Max Fade Factor", &particles_fadeMax, 1, 2000);
			ImGui::SliderFloat("Gravity", &particles_gravity, -2.0f, 1.0f);
			ImGui::SliderFloat("Zoom", &zoom, -60.0f, 60.0f);
			// 添加更多的参数控件...
		}

		if (ImGui::CollapsingHeader("Fog")) {
			// 添加 迷雾 对应的参数控件
			ImGui::Checkbox("Enable", &fog_display);
			ImGui::SliderFloat("Max Distance", &fog_maxdist, 0.0f, 100.0f);
			ImGui::SliderFloat("Min Distance", &fog_mindist, 0.0f, 100.0f);
			ImGui::SliderFloat("Color-Red", &fog_colour_red, 0.0f, 1.0f);
			ImGui::SliderFloat("Color-Green", &fog_colour_green, 0.0f, 1.0f);
			ImGui::SliderFloat("Color-Blue", &fog_colour_blue, 0.0f, 1.0f);
			// 添加更多的参数控件...
		}
		
		if (ImGui::CollapsingHeader("Light")) {
			// 添加 光照 对应的参数控件
				if (ImGui::CollapsingHeader("Ambient Light")) {
					ImGui::Checkbox("Ambient Light Enable", &ambientLight_enable);
					ImGui::SliderFloat("Ambient Light Strength", &ambientLight_strength, 0.0f, 1.0f);
				}
				ImGui::Checkbox("Diffuse Light Enable", &diffuseLight_enable);
				ImGui::Checkbox("Specular Light Enable", &specularLight_enable);

				ImGui::Checkbox("Day and night alternation", &isDayNight);
				ImGui::SliderFloat("Day and night speed", &dayNightSpeed, 0.0f, 50.0f);

				ImGui::Checkbox("Single Light", &isSingleLight);
				if (ImGui::CollapsingHeader("Single Main Light Position")) {
					ImGui::SliderFloat("Light Position X", &lightPos_x, -10.0f, 10.0f);
					ImGui::SliderFloat("Light Position Y", &lightPos_y, -10.0f, 10.0f);
					ImGui::SliderFloat("Light Position Z", &lightPos_z, -10.0f, 10.0f);
				}


				if (ImGui::CollapsingHeader("Kd-Main")) {
					ImGui::SliderFloat("Kd-Main-Red", &kd1_red, 0.0f, 1.0f);
					ImGui::SliderFloat("Kd-Main-Green", &kd1_green, 0.0f, 1.0f);
					ImGui::SliderFloat("Kd-Main-Blue", &kd1_blue, 0.0f, 1.0f);
				}
				if (ImGui::CollapsingHeader("Kd-2")) {
					ImGui::SliderFloat("Kd-2-Red", &kd2_red, 0.0f, 1.0f);
					ImGui::SliderFloat("Kd-2-Green", &kd2_green, 0.0f, 1.0f);
					ImGui::SliderFloat("Kd-2-Blue", &kd2_blue, 0.0f, 1.0f);
				}
				if (ImGui::CollapsingHeader("Kd-3")) {
					ImGui::SliderFloat("Kd-3-Red", &kd3_red, 0.0f, 1.0f);
					ImGui::SliderFloat("Kd-3-Green", &kd3_green, 0.0f, 1.0f);
					ImGui::SliderFloat("Kd-3-Blue", &kd3_blue, 0.0f, 1.0f);
				}
				if (ImGui::CollapsingHeader("Ld-Main")) {
					ImGui::SliderFloat("Ld-Main-Red", &ld1_red, 0.0f, 1.0f);
					ImGui::SliderFloat("Ld-Main-Green", &ld1_green, 0.0f, 1.0f);
					ImGui::SliderFloat("Ld-Main-Blue", &ld1_blue, 0.0f, 1.0f);
				}
				if (ImGui::CollapsingHeader("Ld-2")) {
					ImGui::SliderFloat("Ld-2-Red", &ld2_red, 0.0f, 1.0f);
					ImGui::SliderFloat("Ld-2-Green", &ld2_green, 0.0f, 1.0f);
					ImGui::SliderFloat("Ld-2-Blue", &ld2_blue, 0.0f, 1.0f);
				}
				if (ImGui::CollapsingHeader("Ld-3")) {
					ImGui::SliderFloat("Ld-3Red", &ld3_red, 0.0f, 1.0f);
					ImGui::SliderFloat("Ld-3Green", &ld3_green, 0.0f, 1.0f);
					ImGui::SliderFloat("Ld-3Blue", &ld3_blue, 0.0f, 1.0f);
				}
				if (ImGui::CollapsingHeader("Ld=4")) {
					ImGui::SliderFloat("Ld=4-Red", &ld4_red, 0.0f, 1.0f);
					ImGui::SliderFloat("Ld=4-Green", &ld4_green, 0.0f, 1.0f);
					ImGui::SliderFloat("Ld=4-Blue", &ld4_blue, 0.0f, 1.0f);
				}
			}
			
		if (ImGui::CollapsingHeader("Shadow")) {
			ImGui::Checkbox("Shadows enable", &shadows);
		}

		// 添加 "Exit" 按钮
		if (ImGui::Button("Exit")) {
			// 处理点击 "Exit" 按钮的逻辑
			exit(0);
		}
	}
	ImGui::End();

	ImGui::Render();
	ImGui_ImplOpenGL2_RenderDrawData(ImGui::GetDrawData());
}




// 初始化ImGui和相关的上下文
void initGUI(void*) {
	// 初始化 ImGui 上下文
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();

	// 设置 ImGui 渲染器的回调函数
	ImGui_ImplOpenGL2_Init();
	ImGui_ImplGLUT_Init();

	// 设置 ImGui 的显示尺寸
	io.DisplaySize.x = static_cast<float>(window_width);
	io.DisplaySize.y = static_cast<float>(window_height);

}

// 清理ImGui和相关的上下文
void cleanupGUI() {
	ImGui_ImplOpenGL2_Shutdown();
	ImGui_ImplGLUT_Shutdown();
	ImGui::DestroyContext();
}
