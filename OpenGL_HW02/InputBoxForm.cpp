#include "pch.h"
#include "InputBoxForm.h"

namespace OpenGL_HW02
{
	String^ InputBoxForm::GetInputString()
	{
		return textBox1->Text;
	}

	System::Void InputBoxForm::button1_Click(System::Object^ sender, System::EventArgs^ e)
	{
		this->Close();
	}

	System::Void InputBoxForm::InputBoxForm_Load(System::Object^ sender, System::EventArgs^ e)
	{
		this->CenterToScreen();
	}
}