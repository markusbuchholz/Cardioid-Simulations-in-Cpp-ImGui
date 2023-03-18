// Markus Buchholz, 2023

#include <stdafx.hpp>
#include "imgui_helper.hpp"
#include <tuple>
#include <vector>
#include <math.h>
#include <random>
#include <algorithm>


//---------------------------------------------------------------

bool SIMULATION = true;
float dt = 0.25f;

int w = 1000;
int h = 1000;


//---------------------------------------------------------------

int main(int argc, char const *argv[])
{

	SIMULATION = true;

	std::string title = "Cardioid";
	initImgui(w, h, title);

	bool show_demo_window = true;
	bool show_another_window = false;
	ImVec4 clear_color = ImVec4(89.0f / 255.0, 88.0f / 255.0, 87.0f / 255.0, 1.00f);
	ImVec4 line_color = ImVec4(255.0f / 255.0, 255.0f / 255.0, 255.0f / 255.0, 1.00f);
	ImVec4 blue_color = ImVec4(0.0f / 255.0, 0.0f / 255.0, 170.0f / 255.0, 1.00f);
	ImVec4 dot_color = ImVec4(255.0f / 255.0, 255.0f / 255.0, 0.0f / 255.0, 1.00f);

	float center_x = w / 2.0;
	float center_y = h / 2.0;

	bool flag = true;
	float ang = 0.0f;
	std::vector<ImVec2> tail;

	while (!glfwWindowShouldClose(window) && flag == true)
	{
		glfwPollEvents();

		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();

		ImGui::NewFrame();

		ImGuiWindowFlags window_flags = 0;
		ImGui::SetNextWindowPos(ImVec2(0, 0), ImGuiCond_FirstUseEver);
		ImGui::SetNextWindowSize(ImVec2(w, h), ImGuiCond_FirstUseEver);
		window_flags |= ImGuiWindowFlags_NoMove;
		window_flags |= ImGuiWindowFlags_NoResize;
		window_flags |= ImGuiWindowFlags_NoCollapse;
		window_flags |= ImGuiWindowFlags_NoBackground;
		window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus;

		ImGui::Begin("Simulation", nullptr, window_flags);
		ImDrawList *draw_list = ImGui::GetWindowDrawList();

		float a = 50.0f;

		draw_list->AddCircle({center_x, center_y}, a, ImColor(line_color), 0, 1.0f);
		draw_list->AddCircle({center_x + 2 * a * std::cos(ang * (float)(M_PI / 180.0)), center_y + 2 * a * std::sin(ang * (float)(M_PI / 180.0))}, a, ImColor(line_color), 0, 1.0f);

		float xi = 2.0f * a * (1 - std::cos(ang * M_PI / 180.0f)) * std::cos(ang * M_PI / 180.0f) + center_x + a;
		float yi = 2.0f * a * (1 - std::cos(ang * M_PI / 180.0f)) * std::sin(ang * M_PI / 180.0f) + center_y;

		draw_list->AddCircleFilled({xi, yi}, 5.0f, ImColor(blue_color));

		tail.push_back({xi, yi});

		for (auto &pos : tail)
		{

			draw_list->AddCircleFilled(pos, 1.0, ImColor(blue_color));
		}

		if (ang * (float)(M_PI / 180.0) >= 2 * M_PI)
		{
			tail.erase(tail.begin(), tail.end());
			ang = 0.0f;
		}

		ang += dt;
		ImGui::End();

		// Rendering
		ImGui::Render();
		int display_w, display_h;
		glfwGetFramebufferSize(window, &display_w, &display_h);
		glViewport(0, 0, display_w, display_h);
		glClearColor(clear_color.x, clear_color.y, clear_color.z, clear_color.w);
		glClear(GL_COLOR_BUFFER_BIT);
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		glfwSwapBuffers(window);
	}

	termImgui();
	return 0;
}
