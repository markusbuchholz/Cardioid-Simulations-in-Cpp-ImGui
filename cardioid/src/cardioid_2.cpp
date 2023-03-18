// Markus Buchholz, 2023

#include <stdafx.hpp>
#include "imgui_helper.hpp"
#include <tuple>
#include <vector>
#include <math.h>
#include <algorithm>

//---------------------------------------------------------------

bool SIMULATION = true;
float dt = 0.25f;
int df = 1;
int dfi = 1;
int w = 1000;
int h = 1000;

//---------------------------------------------------------------

struct Dot
{

	float x;
	float y;
	float ang;
};

//---------------------------------------------------------------

int main(int argc, char const *argv[])
{

	SIMULATION = true;

	std::string title = "Cardioid";
	initImgui(w, h, title);

	bool show_demo_window = true;
	bool show_another_window = false;
	ImVec4 clear_color = ImVec4(0.0f / 255.0, 0.0f / 255.0, 0.0f / 255.0, 1.00f);
	ImVec4 line_color = ImVec4(255.0f / 255.0, 255.0f / 255.0, 255.0f / 255.0, 1.00f);
	ImVec4 blue_color = ImVec4(0.0f / 255.0, 0.0f / 255.0, 170.0f / 255.0, 1.00f);
	ImVec4 pink_color = ImVec4(252.0f / 255.0, 3.0f / 255.0, 115.0f / 255.0, 1.00f);
	ImVec4 dot_color = ImVec4(255.0f / 255.0, 255.0f / 255.0, 0.0f / 255.0, 1.00f);

	float center_x = w / 2.0;
	float center_y = h / 2.0;

	bool flag = true;
	float ang = 0.0f;
	std::vector<ImVec2> tail;
	int dotsN = 300;
	int factor = 2;
	std::vector<Dot> dots;
	float a = 300.0f;
	std::vector<std::tuple<Dot, Dot>> edges;
	int counter = 0;

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
		static int factor_i = 2;

		float angi = (float)2 * M_PI / dotsN;

		for (int ii = 0; ii < dotsN; ii++)
		{
			dots.push_back({center_x + a * std::cos(angi * ii), center_y + a * std::sin(angi * ii), angi * ii});
		}

		for (auto &d : dots)
		{

			draw_list->AddCircleFilled({d.x, d.y}, 2.0, ImColor(pink_color));
		}

		for (int ii = 0; ii < dots.size(); ii++)
		{

			float p0x = dots[ii].x;
			float p0y = dots[ii].y;

			int index = (factor * ii) % dotsN;

			float p1x = dots[index].x;
			float p1y = dots[index].y;

			edges.push_back({{p0x, p0y}, {p1x, p1y}});
		}

		for (auto &e : edges)
		{

			ImVec2 p0 = {std::get<0>(e).x, std::get<0>(e).y};
			ImVec2 p1 = {std::get<1>(e).x, std::get<1>(e).y};
			draw_list->AddLine(p0, p1, ImColor(pink_color), 1.0);
		}

		draw_list->AddCircle({center_x, center_y}, a, ImColor(line_color), 0, 1.0f);

		float xi = 2.0f * a * (1 - std::cos(ang * M_PI / 180.0f)) * std::cos(ang * M_PI / 180.0f) + center_x + a;
		float yi = 2.0f * a * (1 - std::cos(ang * M_PI / 180.0f)) * std::sin(ang * M_PI / 180.0f) + center_y;

		counter += dfi;
		if (counter > 50)
		{

			factor += df;
			counter = 0;
		}

		dots.erase(dots.begin(), dots.end());
		edges.erase(edges.begin(), edges.end());

		ImGui::PushItemWidth(300);
		ImGui::SliderInt("factor", &factor_i, 1, 360);

		ImGui::PopItemWidth();

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
