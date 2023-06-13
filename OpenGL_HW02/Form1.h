#pragma once

#include "Scene.h"
#include "Mesh.h"

#include "TextureParamPainter.h"

namespace CppCLRWinformsProject {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;

	public ref class Form1 : public System::Windows::Forms::Form
	{
	private:
		/* Cpp member variables */

		// Main scene
		Scene* scene = nullptr;

		// Mouse
		float lastMouse1X;
		float lastMouse1Y;

		float lastMouse3X;
		float lastMouse3Y;

		TextureParamPainter* painter;

		clock_t lastFrame;

		bool drawSelected = false;
		String^ textureFile;

		/* DotNet tools */

	private: System::Windows::Forms::MenuStrip^ menuStrip1;
	private: System::Windows::Forms::ToolStripMenuItem^ settingToolStripMenuItem;
	private: System::Windows::Forms::ToolStripContainer^ toolStripContainer1;
	private: System::Windows::Forms::ToolStripContainer^ toolStripContainer2;
	private: System::Windows::Forms::ToolStripMenuItem^ faceToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^ pickToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^ deleteToolStripMenuItem;

	private: System::Windows::Forms::ToolStripMenuItem^ testKeyToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^ vertexToolStripMenuItem;

	private: HKOGLPanel::HKOGLPanelControl^ hkoglPanelControl1;
	private: HKOGLPanel::HKOGLPanelControl^ hkoglPanelControl2;
	private: System::Windows::Forms::Button^ calculateButton;
	private: System::Windows::Forms::ToolStripMenuItem^ fileToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^ saveToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^ loadToolStripMenuItem;
	private: System::Windows::Forms::Button^ rotateButton;
	private: System::Windows::Forms::Button^ scaleButton;
	private: System::Windows::Forms::Button^ loadTexButton;

	private: System::Windows::Forms::Button^ newSelectButton;
	private: System::Windows::Forms::OpenFileDialog^ openFileDialog1;



	private: System::ComponentModel::IContainer^ components;

	public:
		Form1(void);

	protected:
		~Form1();

	private:
		std::vector<glm::vec2> getTextureParamVertices();


#pragma region Windows Form Designer generated code
		/* WindowsForm Initializer. (Don't manully modify this function)*/
		void InitializeComponent(void);
#pragma endregion

	private: System::Void hkoglPanelControl1_Load(System::Object^ sender, System::EventArgs^ e);
	private: System::Void hkoglPanelControl1_Paint(System::Object^ sender, System::Windows::Forms::PaintEventArgs^ e);
	private: System::Void hkoglPanelControl1_MouseMove(System::Object^ sender, System::Windows::Forms::MouseEventArgs^ e);
	private: System::Void hkoglPanelControl1_MouseDown(System::Object^ sender, System::Windows::Forms::MouseEventArgs^ e);
	private: System::Void hkoglPanelControl1_MouseWheel(System::Object^ sender, System::Windows::Forms::MouseEventArgs^ e);
	private: System::Void pickToolStripMenuItem_Click(System::Object^ sender, System::EventArgs^ e);
	private: System::Void deleteToolStripMenuItem_Click(System::Object^ sender, System::EventArgs^ e);
	private: System::Void vertexToolStripMenuItem_Click(System::Object^ sender, System::EventArgs^ e);
	private: System::Void testKeyToolStripMenuItem_Click(System::Object^ sender, System::EventArgs^ e);
	private: System::Void hkoglPanelControl2_Load(System::Object^ sender, System::EventArgs^ e);
	private: System::Void hkoglPanelControl2_Paint(System::Object^ sender, System::Windows::Forms::PaintEventArgs^ e);
	private: System::Void calculateButton_Click(System::Object^ sender, System::EventArgs^ e);
	private: System::Void rotateButton_Click(System::Object^ sender, System::EventArgs^ e);
	private: System::Void scaleButton_Click(System::Object^ sender, System::EventArgs^ e);
	private: System::Void loadTexButton_Click(System::Object^ sender, System::EventArgs^ e);
	private: System::Void newSelectButton_Click(System::Object^ sender, System::EventArgs^ e);
	private: System::Void saveToolStripMenuItem_Click(System::Object^ sender, System::EventArgs^ e);
	private: System::Void loadToolStripMenuItem_Click(System::Object^ sender, System::EventArgs^ e);
	
	};
}
