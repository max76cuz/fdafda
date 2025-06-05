#include "ui.h"

void custom::TitleBar(const std::string WindowName)
{

    ImGui::PushStyleColor(ImGuiCol_::ImGuiCol_ChildBg, color::Background_Gray);
    ImGui::PushStyleColor(ImGuiCol_::ImGuiCol_Button, color::Background_Gray);
    ImGui::PushStyleColor(ImGuiCol_::ImGuiCol_Text, color::Text_Blue);
    ImGui::PushFont(font::Poppins_SemiBold);

    ImGui::SetCursorPos({ 0,0 });

    ImVec2 CurrentMousePos;
    const ImVec2 TitleBarSize = { 500,35 };
    ImGui::SetNextWindowSize(TitleBarSize);
    ImGui::BeginChild("TitleBar", TitleBarSize, ImGuiChildFlags_::ImGuiChildFlags_Border | ImGuiWindowFlags_::ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoResize);

    CurrentMousePos = ImGui::GetCursorPos();
    ImGui::SetCursorPos({ CurrentMousePos.x, 8 });

    ImGui::Text(WindowName.c_str());
    
    ImGui::PopStyleColor(3);
    ImGui::PopFont();
}

bool custom::button(const char* label, ImVec2 pos, ImVec2 size)
{
    auto d = ImGui::GetWindowDrawList();
    ImGui::PushStyleColor(ImGuiCol_Button, { 0.1333, 0.1373, 0.1373, 1.0000 });
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, { 0.1333, 0.1373, 0.1373, 0.9 });
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, { 0.1333, 0.1373, 0.1373, 1 });

    ImGui::SetCursorPos(pos);
    bool result = ImGui::Button(label, size);

    ImVec2 MIN = ImGui::GetItemRectMin();
    ImVec2 MAX = ImGui::GetItemRectMax();

    d->AddRect(MIN, MAX, ImGui::GetColorU32(color::Border_Color), 0, 0, 2.f);
    d->AddLine({ MIN.x - 1, MAX.y + 2 }, { MAX.x + 1, MAX.y + 2 }, ImColor(0, 0, 0, 255));

    ImGui::PopStyleColor(3);
    return result;
}

bool custom::Button(std::string name)
{
    ImGuiWindow* window = ImGui::GetCurrentWindow();
    ImGuiID id = window->GetID(name.c_str());
    const ImVec2 textSize = ImGui::CalcTextSize(name.c_str());

    const ImVec2 size = { textSize.x + 8, textSize.y + 8 }; 

    const ImRect rect(window->DC.CursorPos, window->DC.CursorPos + size);
    ImGuiContext& g = *GImGui;
    const ImGuiStyle& style = g.Style;

    ImGui::ItemSize(rect, style.FramePadding.y);
    if (!ImGui::ItemAdd(rect, id))
        return false;

    bool hovered, held;
    bool pressed = ImGui::ButtonBehavior(rect, id, &hovered, &held);

    ImU32 backgroundColor = ImGui::GetColorU32(hovered ? ImGuiCol_ButtonHovered : ImGuiCol_Button);
    window->DrawList->AddRectFilled(rect.Min, rect.Max, backgroundColor, style.FrameRounding);

    ImVec2 textPos = { rect.Min.x + style.FramePadding.x, rect.Min.y + (size.y - textSize.y) * 0.5f }; 
    window->DrawList->AddText(textPos, ImGui::GetColorU32(ImGuiCol_Text), name.c_str());

    return pressed;
}

bool custom::TabButton(const char* label, ImVec2 pos, bool Toggled)
{
    ImGui::PushStyleColor(ImGuiCol_Button, color::TabButton::ButtonColor);
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, color::TabButton::ButtonColorHovered);
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, color::TabButton::ButtonColorActive);
    ImVec4 CurrentColor;

    if (Toggled)
    {
        CurrentColor = color::Text_Blue;
    }
    else
    {
        CurrentColor = color::Text_Gray;
    }

    ImGui::PushStyleColor(ImGuiCol_Text, CurrentColor);

    ImGui::PushFont(font::Tab_Poppins_SemiBold);


    ImGui::SetCursorPos(pos);
    bool result = custom::Button(label);


    ImGui::PopStyleColor(4);
    ImGui::PopFont();
    return result;
}

bool custom::TabButton(const char* label, ImVec2 pos)
{
    ImGui::PushStyleColor(ImGuiCol_Button, color::TabButton::ButtonColor);
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, color::TabButton::ButtonColorHovered);
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, color::TabButton::ButtonColorActive);
    ImGui::PushFont(font::Tab_Poppins_SemiBold);

    ImGui::PushStyleColor(ImGuiCol_Text, color::Text_Gray);
    ImGui::SetCursorPos(pos);
    bool result = ImGui::Button(label, ButtonSize);

    ImGui::PopStyleColor(4);
    ImGui::PopFont();
    return result;
}

void custom::Begin()
{
    ImGui::SetNextWindowSize(custom::WindowSize);
    ImGui::Begin("MENU", NULL, ImGuiWindowFlags_::ImGuiWindowFlags_NoResize | ImGuiWindowFlags_::ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_::ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_::ImGuiWindowFlags_NoScrollbar);
}

void custom::Child(const char* Name, ImVec2 Pos, ImVec2 Size)
{
    ImGui::PushStyleColor(ImGuiCol_ChildBg, color::Background_Black);
    ImGui::SetCursorPos(Pos);
    ImGui::BeginChild(Name, Size, ImGuiWindowFlags_::ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoResize);
    ImGui::PopStyleColor();
}

void custom::BottomBar(const std::string Info)
{
    ImGui::PushStyleColor(ImGuiCol_ChildBg, color::Background_Gray);
    ImGui::PushStyleColor(ImGuiCol_::ImGuiCol_Text, color::Text_Blue);
    ImGui::PushFont(font::Poppins_SemiBold);

    ImGui::SetCursorPos({ 0,475 });
    ImGui::BeginChild("BOTTOM", { 500,25 }, ImGuiChildFlags_::ImGuiChildFlags_Border | ImGuiWindowFlags_NoResize);
    ImGui::Text(Info.c_str());

    ImGui::PopStyleColor(2);
    ImGui::PopFont();
    ImGui::EndChild();
}

void custom::BottomBar(const std::string Info, std::string MoreInfo)
{
    ImGui::PushStyleColor(ImGuiCol_ChildBg, color::Background_Gray);
    ImGui::PushStyleColor(ImGuiCol_::ImGuiCol_Text, color::Text_Blue);

    ImGui::SetCursorPos({ 0,475 });
    ImGui::BeginChild("BOTTOM", { 500,25 }, ImGuiChildFlags_::ImGuiChildFlags_Border | ImGuiWindowFlags_NoResize);

    ImGui::PushFont(font::Poppins_SemiBold);
    ImGui::Text(Info.c_str());
    ImGui::PopFont();

    ImGui::PopStyleColor(2);
   
    ImGui::PushStyleColor(ImGuiCol_::ImGuiCol_Text, color::Text_Gray);
    ImGui::PushFont(font::Poppins);

    ImGui::SameLine();
    float textWidth = ImGui::CalcTextSize(MoreInfo.c_str()).x;
    float startX = 500.0f - 10.0f - textWidth;
    ImGui::SetCursorPosX(startX);
    ImGui::PushTextWrapPos(500.0f - 10.0f);
    ImGui::Text(MoreInfo.c_str());
    ImGui::PopTextWrapPos();



    ImGui::PopStyleColor();
    ImGui::PopFont();
    ImGui::EndChild();
}

void custom::BeginSection(const std::string SectionName, ImVec2 Pos, ImVec2 Size)
{
    ImGui::SetCursorPos(Pos);

    ImGui::BeginChild(SectionName.c_str(), Size, true, ImGuiChildFlags_Border);

    ImGui::PushStyleColor(ImGuiCol_ChildBg, color::Background_Gray); 
    ImGui::PushStyleColor(ImGuiCol_Text, color::Text_Blue);           

    float headerHeight = 30.0f; 
    ImVec2 headerSize = { Size.x, headerHeight };
    ImGui::SetCursorPos({ ImGui::GetCursorPosX() - 7.f ,ImGui::GetCursorPosY() - 3.5f});
    ImGui::BeginChild(SectionName.c_str(), headerSize, true, ImGuiChildFlags_Border);

    ImGui::SetCursorPos({ 5.0f, 6.0f }); 

    ImGui::PushFont(font::Tab_Poppins_SemiBold);
    ImGui::Text(SectionName.c_str());
    ImGui::PopFont();

    ImGui::PopStyleColor(2); 

    ImGui::EndChild();

    ImGui::SetCursorPosY(ImGui::GetCursorPosY()); 
}

bool custom::CheckBox(const char* label, bool* v)
{
   // ImGui::PushStyleVar(ImGuiStyleVar_FramePadding,{ 0.699999809265137f,1.f});
    ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 0.1f);
    ImGui::PushStyleColor(ImGuiCol_Text, color::Text_Gray);
    ImGui::PushFont(font::Poppins_SemiBold_txt);

    ImGuiWindow* window = ImGui::GetCurrentWindow();
    if (window->SkipItems)
        return false;


    ImGuiContext& g = *GImGui;
    const ImGuiStyle& style = g.Style;
    const ImGuiID id = window->GetID(label);
    const ImVec2 label_size = ImGui::CalcTextSize(label, NULL, true);

    const float square_sz = ImGui::GetFrameHeight() - 12.5f;
    const ImVec2 pos = window->DC.CursorPos;
    const ImRect total_bb(pos, pos + ImVec2(square_sz + (label_size.x > 0.0f ? style.ItemInnerSpacing.x + label_size.x : 0.0f), label_size.y + style.FramePadding.y - 6.f));
    ImGui::ItemSize(total_bb, style.FramePadding.y);
    const bool is_visible = ImGui::ItemAdd(total_bb, id);
    const bool is_multi_select = (g.LastItemData.ItemFlags & ImGuiItemFlags_IsMultiSelect) != 0;

    if (!is_visible)
        return false;

    bool checked = *v;
    if (is_multi_select)
        ImGui::MultiSelectItemHeader(id, &checked, NULL);

    bool hovered, held;
    bool pressed = ImGui::ButtonBehavior(total_bb, id, &hovered, &held);

    if (is_multi_select)
        ImGui::MultiSelectItemFooter(id, &checked, &pressed);
    else if (pressed)
        checked = !checked;

    if (*v != checked)
    {
        *v = checked;
        pressed = true;
        ImGui::MarkItemEdited(id);
    }



    const ImRect check_bb(pos, pos + ImVec2(square_sz, square_sz));
    const bool mixed_value = (g.LastItemData.ItemFlags & ImGuiItemFlags_MixedValue) != 0;
    if (is_visible)
    {
        ImGui::RenderNavCursor(total_bb, id);
        ImGui::RenderFrame(check_bb.Min, check_bb.Max, ImGui::GetColorU32((held && hovered) ? ImGuiCol_FrameBgActive : hovered ? ImGuiCol_FrameBgHovered : ImGuiCol_FrameBg), true, style.FrameRounding);
        ImU32 check_col = ImColor(1, 1, 1);

        if (mixed_value)
        {
            ImVec2 pad(ImMax(1.0f, IM_TRUNC(square_sz / 3.6f)), ImMax(1.0f, IM_TRUNC(square_sz / 3.6f)));
            window->DrawList->AddRectFilled(check_bb.Min + pad, check_bb.Max - pad, check_col, style.FrameRounding);
        }
        else if (*v)
        {
            const float pad = ImMax(1.0f, IM_TRUNC(square_sz / 4.0f));
            window->DrawList->AddRectFilled(check_bb.Min, check_bb.Max, ImColor(115, 115, 255), style.FrameRounding);
            ImGui::RenderCheckMark(window->DrawList, check_bb.Min + ImVec2(pad, pad), check_col, square_sz - pad * 2.0f);
        }
    }

    const float label_offset_x = style.ItemInnerSpacing.x - 1.f;
    const float label_offset_y = style.FramePadding.y;
    const ImVec2 label_pos = ImVec2(check_bb.Max.x + label_offset_x, check_bb.Min.y + (check_bb.GetHeight() - label_size.y + 2.5) * 0.5f);


    if (is_visible && label_size.x > 0.0f)
        ImGui::RenderText(label_pos, label);

    ImGui::PopFont();
    ImGui::PopStyleVar();
    ImGui::PopStyleColor();

    return pressed;
}

bool custom::Tab(const char* TabName, int Tab)
{
    ImVec2 TabButtonPos;

    float WindowWidth = ImGui::GetWindowWidth();

    ImVec2 TextSize = ImGui::CalcTextSize(TabName);

    float TabWidth = ImMax(60.0f, TextSize.x);  
    
    float CurrentOffset = TabWidth * Tab;

    TabButtonPos = { WindowWidth - CurrentOffset - TabWidth, 5 };

    return custom::TabButton(TabName, TabButtonPos);
}

void UI::UI()
{
    if (GetAsyncKeyState(VK_INSERT) & 1) {
        UI::ShowMenu = !UI::ShowMenu;
    }

    if (UI::ShowMenu) {

        custom::Begin();
        custom::TitleBar("moneyhack return");

        if (custom::Tab("Aimbot", 1)) { custom::CurrentTab = custom::Tabs::Aimbot; }
        if (custom::Tab("Visuals", 2)) { custom::CurrentTab = custom::Tabs::Visuals; }
        if (custom::Tab("Misc", 3)) { custom::CurrentTab = custom::Tabs::Misc; }
        if (custom::Tab("Settings", 4)) { custom::CurrentTab = custom::Tabs::Settings; }

        ImGui::EndChild();

        ImVec2 FrameSize = { 500, 430 };
        ImGui::SetCursorPosX(0);
        custom::Child("FRAME", ImGui::GetCursorPos(), FrameSize);

        ImVec2 currentPos;

        switch (custom::CurrentTab)
        {
        case custom::Tabs::Aimbot:
            
            currentPos = ImGui::GetCursorPos();
            custom::BeginSection("Aimbot", { ImGui::GetCursorPos() }, { 235, 300 });
         
            static bool test;
            custom::CheckBox("Aimbot", &test);
            custom::CheckBox("Rage-Bot", &test);
            custom::CheckBox("Prediction", &test);

            ImGui::EndChild();

            ImGui::SetCursorPosX(currentPos.x + 240);
            ImGui::SetCursorPosY(currentPos.y);
            custom::BeginSection("Settings", { ImGui::GetCursorPos() }, { 245, 300 });
          

            custom::CheckBox("Sensitivity", &test);
            ImGui::Text(("FOV"));
			ImGui::SliderInt(("##FOV"), &FOV, 65, 90);
            
			if (Sc == true) {

					script();
			}

            ImGui::SliderFloat(("##Sensitivity"),
            ImGui::EndChild();

            break;
        case custom::Tabs::Visuals:
            ImGui::Text("Visuals tab");
            break;

        case custom::Tabs::Misc:
            ImGui::Text("Misc tab");
            break;

        case custom::Tabs::Settings:
            ImGui::Text("Settings tab");
            break;



        default:
            break;
        }


        ImGui::EndChild();


        custom::BottomBar("@iraqichild", "Expiry: October 13 2025");




        ImGui::End();
    }
}
