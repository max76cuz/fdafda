#ifndef UI_H
#define UI_H
#include "../libs/imgui/imgui.h"
#include "../libs/imgui/imgui_internal.h"
#include "../libs/imgui/imgui_impl_win32.h"
#include "../libs/imgui/imgui_impl_dx11.h"
#include "font.h"
#include <string>
#include <thread> 
#include <mutex>  
#define WIN32_MEAN_AND_LEAN
#include <Windows.h>
#include <vector>

#pragma warning(push)  
#pragma warning(disable: 4244)  
#pragma warning(disable: 4267)



namespace custom
{
	namespace font
	{
		inline ImFont* Poppins;
		inline ImFont* Poppins_SemiBold;
		inline ImFont* Poppins_SemiBold_txt;
		inline ImFont* Tab_Poppins_SemiBold;

		inline void LoadFonts()
		{
			ImGuiIO& io = ImGui::GetIO();
			io.Fonts->AddFontDefault();
			Poppins_SemiBold = io.Fonts->AddFontFromMemoryTTF(f_Poppins_SemiBold, sizeof(f_Poppins_SemiBold), 20.f);
			Poppins_SemiBold_txt = io.Fonts->AddFontFromMemoryTTF(f_Poppins_SemiBold, sizeof(f_Poppins_SemiBold), 16.f);
			Tab_Poppins_SemiBold = io.Fonts->AddFontFromMemoryTTF(f_Poppins_SemiBold, sizeof(f_Poppins_SemiBold), 17.f);
			Poppins = io.Fonts->AddFontFromMemoryTTF(f_Poppins, sizeof(f_Poppins), 16.f);
			io.Fonts->Build();
		}
	}

	namespace color
	{
		inline ImVec4 Background_Gray = ImVec4(10.0f / 255.f, 10 / 255.f, 10 / 255.f, 1.0f);
		inline ImVec4 Background_Black = ImVec4(9.999899930335232e-07f, 9.999935173254926e-07f, 9.999999974752427e-07f, 1.0f);
		inline ImVec4 Border_Color = { 15 / 255.f, 15 / 255, 15 / 255, 1.0f };
		inline ImVec4 Text_Blue = { 65.0f / 255.f, 65.0f / 255.f, 255.0f / 255.f, 1.0 };
		inline ImVec4 Text_Gray = { 80.0f / 255.f, 80.0f / 255.f, 80.0f / 255.f, 1.0f };
		inline ImVec4 Text_White = { 255.0f / 255.f, 255.0f / 255.f, 255.0f / 255.f, 1.0f };


		namespace TabButton
		{
			inline const ImVec4 ButtonColor = { 15.0f / 255.f, 15.0f / 255.f, 15.0f / 255.f, 0.1f };
			inline const ImVec4 ButtonColorHovered = { 15.0f / 255.f, 15.0f / 255.f, 15.0f / 255.f, 0.5f };
			inline const ImVec4 ButtonColorActive = { 15.0f / 255.f, 15.0f / 255.f, 15.0f / 255.f, 0.1f };
		}
	}

	const ImVec2 WindowSize = { 500,500 };
	const ImVec2 ButtonSize = { 50,25 };

	inline void InitUI()
	{
		font::LoadFonts();

		ImGuiStyle* style = &ImGui::GetStyle();

		style->Alpha = 1.0f;
		style->DisabledAlpha = 0.4000000059604645f;
		style->WindowPadding = ImVec2(7.300000190734863f, 3.0f);
		style->WindowRounding = 2.0f;
		style->WindowBorderSize = 1.f;
		style->WindowMinSize = ImVec2(20.0f, 20.0f);
		style->WindowTitleAlign = ImVec2(0.0f, 0.0f);
		style->WindowMenuButtonPosition = ImGuiDir_None;
		style->ChildRounding = 0.699999988079071f;
		style->ChildBorderSize = 1.0f;
		style->PopupRounding = 0.0f;
		style->PopupBorderSize = 1.0f;
		style->Colors[ImGuiCol_FrameBg] = ImVec4(9.999899930335232e-07f, 9.999956773754093e-07f, 9.999999974752427e-07f, 1.0f);
		style->Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.08288969844579697f, 0.08387214690446854f, 0.08583688735961914f, 1.0f);
		style->Colors[ImGuiCol_FrameBgActive] = ImVec4(0.08288969844579697f, 0.08387214690446854f, 0.08583688735961914f, 1.0f);
		style->ItemSpacing = ImVec2(12.10000038146973f, 5.599999904632568f);
		style->ItemInnerSpacing = ImVec2(7.699999809265137f, 3.0f);
		style->CellPadding = ImVec2(20.0f, 11.69999980926514f);
		style->IndentSpacing = 5.599999904632568f;
		style->ColumnsMinSpacing = 4.599999904632568f;
		style->ScrollbarSize = 12.0f;
		style->ScrollbarRounding = 0.0f;
		style->GrabMinSize = 5.0f;
		style->GrabRounding = 0.0f;
		style->TabRounding = 1.5f;
		style->TabBorderSize = 0.0f;
		style->TabMinWidthForCloseButton = 0.0f;
		style->ColorButtonPosition = ImGuiDir_Right;
		style->ButtonTextAlign = ImVec2(0.5f, 0.5f);
		style->SelectableTextAlign = ImVec2(0.0f, 0.0f);
		style->FramePadding = ImVec2(0.699999809265137f, 6.900000095367432f);
		style->FrameRounding = 2.199999809265137f;
		style->FrameBorderSize = 0.0f;

		style->Colors[ImGuiCol_Text] = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
		style->Colors[ImGuiCol_TextDisabled] = ImVec4(0.3148704171180725f, 0.3157033920288086f, 0.3175965547561646f, 1.0f);
		style->Colors[ImGuiCol_WindowBg] = ImVec4(9.999899930335232e-07f, 9.999935173254926e-07f, 9.999999974752427e-07f, 1.0f);
		style->Colors[ImGuiCol_ChildBg] = ImVec4(9.999899930335232e-07f, 9.999932899518171e-07f, 9.999999974752427e-07f, 1.0f);
		style->Colors[ImGuiCol_PopupBg] = ImVec4(0.0784313753247261f, 0.08627451211214066f, 0.1019607856869698f, 1.0f);
		style->Colors[ImGuiCol_Border] = ImVec4(0.1030042767524719f, 0.09195231646299362f, 0.09195231646299362f, 1.0f);
		style->Colors[ImGuiCol_BorderShadow] = ImVec4(0.0784313753247261f, 0.08627451211214066f, 0.1019607856869698f, 1.0f);
		style->Colors[ImGuiCol_FrameBg] = ImVec4(9.999899930335232e-07f, 9.99995791062247e-07f, 9.999999974752427e-07f, 1.0f);
		style->Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.05363886803388596f, 0.05435740575194359f, 0.05579400062561035f, 1.0f);
		style->Colors[ImGuiCol_FrameBgActive] = ImVec4(0.2352941185235977f, 0.2156862765550613f, 0.5960784554481506f, 1.0f);
		style->Colors[ImGuiCol_TitleBg] = ImVec4(0.0470588244497776f, 0.05490196123719215f, 0.07058823853731155f, 1.0f);
		style->Colors[ImGuiCol_TitleBgActive] = ImVec4(0.0470588244497776f, 0.05490196123719215f, 0.07058823853731155f, 1.0f);
		style->Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.0784313753247261f, 0.08627451211214066f, 0.1019607856869698f, 1.0f);
		style->Colors[ImGuiCol_MenuBarBg] = ImVec4(0.09803921729326248f, 0.105882354080677f, 0.1215686276555061f, 1.0f);
		style->Colors[ImGuiCol_ScrollbarBg] = ImVec4(0.0470588244497776f, 0.05490196123719215f, 0.07058823853731155f, 1.0f);
		style->Colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.1176470592617989f, 0.1333333402872086f, 0.1490196138620377f, 1.0f);
		style->Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.1568627506494522f, 0.168627455830574f, 0.1921568661928177f, 1.0f);
		style->Colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.1176470592617989f, 0.1333333402872086f, 0.1490196138620377f, 1.0f);
		style->Colors[ImGuiCol_CheckMark] = ImVec4(0.4980392158031464f, 0.5137255191802979f, 1.0f, 1.0f);
		style->Colors[ImGuiCol_SliderGrab] = ImVec4(0.4980392158031464f, 0.5137255191802979f, 1.0f, 1.0f);
		style->Colors[ImGuiCol_SliderGrabActive] = ImVec4(0.5372549295425415f, 0.5529412031173706f, 1.0f, 1.0f);
		style->Colors[ImGuiCol_Button] = ImVec4(9.999899930335232e-07f, 9.999948815675452e-07f, 9.999999974752427e-07f, 1.0f);
		style->Colors[ImGuiCol_ButtonHovered] = ImVec4(0, 0, 0, 0.0f);
		style->Colors[ImGuiCol_ButtonActive] = ImVec4(9.999899930335232e-07f, 9.999948815675452e-07f, 9.999999974752427e-07f, 0.3f);
		style->Colors[ImGuiCol_Header] = ImVec4(0.01716721802949905f, 0.01716730929911137f, 0.01716738939285278f, 1.0f);
		style->Colors[ImGuiCol_HeaderHovered] = ImVec4(0.09020004421472549f, 0.08996298909187317f, 0.09442061185836792f, 1.0f);
		style->Colors[ImGuiCol_HeaderActive] = ImVec4(0.2352941185235977f, 0.2156862765550613f, 0.5960784554481506f, 1.0f);
		style->Colors[ImGuiCol_Separator] = ImVec4(0.1568627506494522f, 0.1843137294054031f, 0.250980406999588f, 1.0f);
		style->Colors[ImGuiCol_SeparatorHovered] = ImVec4(0.1568627506494522f, 0.1843137294054031f, 0.250980406999588f, 1.0f);
		style->Colors[ImGuiCol_SeparatorActive] = ImVec4(0.1568627506494522f, 0.1843137294054031f, 0.250980406999588f, 1.0f);
		style->Colors[ImGuiCol_ResizeGrip] = ImVec4(0.1176470592617989f, 0.1333333402872086f, 0.1490196138620377f, 1.0f);
		style->Colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.196078434586525f, 0.1764705926179886f, 0.5450980663299561f, 1.0f);
		style->Colors[ImGuiCol_ResizeGripActive] = ImVec4(0.2352941185235977f, 0.2156862765550613f, 0.5960784554481506f, 1.0f);
		style->Colors[ImGuiCol_Tab] = ImVec4(0.09441966563463211f, 0.09441998600959778f, 0.09442061185836792f, 1.0f);
		style->Colors[ImGuiCol_TabHovered] = ImVec4(0.1176470592617989f, 0.1333333402872086f, 0.1490196138620377f, 1.0f);
		style->Colors[ImGuiCol_TabActive] = ImVec4(0.09803921729326248f, 0.105882354080677f, 0.1215686276555061f, 1.0f);
		style->Colors[ImGuiCol_TabUnfocused] = ImVec4(0.0470588244497776f, 0.05490196123719215f, 0.07058823853731155f, 1.0f);
		style->Colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.0784313753247261f, 0.08627451211214066f, 0.1019607856869698f, 1.0f);
		style->Colors[ImGuiCol_TableHeaderBg] = ImVec4(0.0135570801794529f, 0.01478669699281454f, 0.01716738939285278f, 1.0f);
		style->Colors[ImGuiCol_TableBorderStrong] = ImVec4(0.0470588244497776f, 0.05490196123719215f, 0.07058823853731155f, 1.0f);
		style->Colors[ImGuiCol_TableBorderLight] = ImVec4(0.0f, 0.0f, 0.0f, 1.0f);
		style->Colors[ImGuiCol_TableRowBg] = ImVec4(0.1176470592617989f, 0.1333333402872086f, 0.1490196138620377f, 1.0f);
		style->Colors[ImGuiCol_TableRowBgAlt] = ImVec4(0.09803921729326248f, 0.105882354080677f, 0.1215686276555061f, 1.0f);
		style->Colors[ImGuiCol_TextSelectedBg] = ImVec4(0.2352941185235977f, 0.2156862765550613f, 0.5960784554481506f, 1.0f);
	}

    static enum Tabs
	{
		Aimbot,
		Visuals,
		Misc,
		Settings
	}CurrentTab;



	bool CheckBox(const char* label, bool* v);
	bool Button(std::string name);
	void TitleBar(const std::string WindowName);
	void BottomBar(const std::string Info);
	void BottomBar(const std::string Info, std::string MoreInfo);
	bool button(const char* label, ImVec2 pos, ImVec2 size);
	bool TabButton(const char* label, ImVec2 pos, bool Toggled);
	bool TabButton(const char* label, ImVec2 pos);
	void BeginSection(const std::string SectionName, ImVec2 Pos, ImVec2 Size);
	void Begin();
	void Child(const char* Name, ImVec2 Pos, ImVec2 Size);

	bool Tab(const char* TabName, int Tab);

	
}

namespace UI
{
	static bool ShowMenu = true;

	void UI();
}

#pragma warning(pop)

#endif // !UI_H

