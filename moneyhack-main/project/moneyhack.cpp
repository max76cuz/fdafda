#include "includes.h"

int main()
{

	if (!(overlay::SetupWindow()))
		return 1;

	custom::InitUI();

	while (!(overlay::ShouldQuit))
	{
		overlay::Render();

		UI::UI();

		//auto& io = ImGui::GetIO();
		//ImVec2 center = ImVec2(io.MousePos.x, io.MousePos.y);
		//ImGui::GetBackgroundDrawList()->AddCircle(center, 60, ImColor(255, 255, 255), 256, 1.f);
		
		overlay::EndRender();
	}

	return 0;
}