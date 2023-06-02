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
	using namespace System::Collections::Generic;

	/// <summary>
	/// Zusammenfassung für Form1
	/// </summary>
	public ref class Form1 : public Form
	{
	private:
		Scene* scene = nullptr;
		clock_t deltaTime = 0;
		clock_t lastFrame = 0;

		List<GroupBox^>^ groupBoxesList;
		List<CheckBox^>^ groupCheckBoxesList;
		List<ComboBox^>^ groupComboBoxesList;

		/* Windows Form */

		// Setting UI Menu
		MenuStrip^ menuStrip1;
		ToolStripMenuItem^ settingToolStripMenuItem;
		ToolStripContainer^ toolStripContainer1;
		ToolStripContainer^ toolStripContainer2;
		ToolStripMenuItem^ faceToolStripMenuItem;
		ToolStripMenuItem^ pickToolStripMenuItem;
		ToolStripMenuItem^ deleteToolStripMenuItem;
		ToolStripMenuItem^ vertexToolStripMenuItem;

		// Group UI Pannel
		FlowLayoutPanel^ groupLayoutPannel;

		// Dynamic UIs
		Button^ addGroupButton;

		// Background Worker
		BackgroundWorker^ backgroundWorker1;

		// Others
		System::Windows::Forms::Timer^ timer1;
		IContainer^ components;



		// OpenGL
		HKOGLPanel::HKOGLPanelControl^ hkoglPanelControl1;


	public:
		Form1(void);

	protected:
		/// <summary>
		/// Verwendete Ressourcen bereinigen.
		/// </summary>
		~Form1();

	protected:

	private:
		/// <summary>
		/// Erforderliche Designervariable.
		/// </summary>

		Button^ CreateGroupRemoveButton();
		CheckBox^ CreateGroupSelectCheckBox();
		ComboBox^ CreateGroupColorComboBox();

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
	private: System::Void onAddGroupButtonClick(System::Object^ sender, System::EventArgs^ e);
	private: System::Void onGroupDeleteButtonClick(System::Object^ sender, System::EventArgs^ e);
	private: System::Void Form1_Load(System::Object^ sender, System::EventArgs^ e) {
	}
	private: System::Void onGroupCheckBoxClicked(System::Object^ sender, System::EventArgs^ e);
	private: System::Void onGroupComboBoxIndexChanged(System::Object^ sender, System::EventArgs^ e);
	};
}
