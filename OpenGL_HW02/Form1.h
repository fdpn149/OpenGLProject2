#pragma once

#include "Scene.h"
#include "Mesh.h"

namespace CppCLRWinformsProject {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;

	/// <summary>
	/// Zusammenfassung für Form1
	/// </summary>
	public ref class Form1 : public System::Windows::Forms::Form
	{
		Graphics^ graph;
		Pen^ pen;
		Scene* scene = nullptr;
		clock_t deltaTime = 0;
		clock_t lastFrame = 0;
	private: System::Windows::Forms::MenuStrip^ menuStrip1;
	private: System::Windows::Forms::ToolStripMenuItem^ settingToolStripMenuItem;
	private: System::Windows::Forms::ToolStripContainer^ toolStripContainer1;
	private: System::Windows::Forms::ToolStripContainer^ toolStripContainer2;
	private: System::Windows::Forms::ToolStripMenuItem^ faceToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^ pickToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^ deleteToolStripMenuItem;
	private: System::Windows::Forms::Panel^ panel1;
	private: System::Windows::Forms::ToolStripMenuItem^ testKeyToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^ vertexToolStripMenuItem;
	public:
		Form1(void);

	protected:
		/// <summary>
		/// Verwendete Ressourcen bereinigen.
		/// </summary>
		~Form1();

	private: System::Windows::Forms::Timer^ timer1;
	private: HKOGLPanel::HKOGLPanelControl^ hkoglPanelControl1;
	private: System::ComponentModel::IContainer^ components;
	protected:

	private:
		/// <summary>
		/// Erforderliche Designervariable.
		/// </summary>


#pragma region Windows Form Designer generated code
		/// <summary>
		/// Erforderliche Methode für die Designerunterstützung.
		/// Der Inhalt der Methode darf nicht mit dem Code-Editor geändert werden.
		/// </summary>
		void InitializeComponent(void);
#pragma endregion

	private: System::Void hkoglPanelControl1_Load(System::Object^ sender, System::EventArgs^ e);
	private: System::Void hkoglPanelControl1_Paint(System::Object^ sender, System::Windows::Forms::PaintEventArgs^ e);
	private: System::Void timer1_Tick(System::Object^ sender, System::EventArgs^ e);
	private: System::Void hkoglPanelControl1_MouseMove(System::Object^ sender, System::Windows::Forms::MouseEventArgs^ e);
	private: System::Void hkoglPanelControl1_MouseUp(System::Object^ sender, System::Windows::Forms::MouseEventArgs^ e);
	private: System::Void hkoglPanelControl1_KeyPress(System::Object^ sender, System::Windows::Forms::KeyPressEventArgs^ e);
	private: System::Void hkoglPanelControl1_MouseDown(System::Object^ sender, System::Windows::Forms::MouseEventArgs^ e);
	private: System::Void hkoglPanelControl1_MouseWheel(System::Object^ sender, System::Windows::Forms::MouseEventArgs^ e);
	private: System::Void pickToolStripMenuItem_Click(System::Object^ sender, System::EventArgs^ e);
	private: System::Void deleteToolStripMenuItem_Click(System::Object^ sender, System::EventArgs^ e);
	private: System::Void vertexToolStripMenuItem_Click(System::Object^ sender, System::EventArgs^ e);
	private: System::Void testKeyToolStripMenuItem_Click(System::Object^ sender, System::EventArgs^ e);
		   PointF transCoord(TriMesh::Point point);
	};
}
