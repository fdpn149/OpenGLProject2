#include "pch.h"
#include "Form1.h"

// #include "Form1.h"
#define GLEW_STATIC
#include <GL/glew.h>
#include <iostream>

CppCLRWinformsProject::Form1::Form1(void)
{
	InitializeComponent();

	// My types
	this->groupBoxesList = (gcnew List<GroupBox^>());
}

/// <summary>
/// Verwendete Ressourcen bereinigen.
/// </summary>

CppCLRWinformsProject::Form1::~Form1()
{
	delete scene;
	if (components)
	{
		delete components;
	}
}

/// <summary>
/// Erforderliche Methode für die Designerunterstützung.
/// Der Inhalt der Methode darf nicht mit dem Code-Editor geändert werden.
/// </summary>

void CppCLRWinformsProject::Form1::InitializeComponent(void)
{
	this->components = (gcnew System::ComponentModel::Container());
	HKOGLPanel::HKCOGLPanelCameraSetting^ hkcoglPanelCameraSetting2 = (gcnew HKOGLPanel::HKCOGLPanelCameraSetting());
	HKOGLPanel::HKCOGLPanelPixelFormat^ hkcoglPanelPixelFormat2 = (gcnew HKOGLPanel::HKCOGLPanelPixelFormat());
	this->timer1 = (gcnew System::Windows::Forms::Timer(this->components));
	this->hkoglPanelControl1 = (gcnew HKOGLPanel::HKOGLPanelControl());
	this->menuStrip1 = (gcnew System::Windows::Forms::MenuStrip());
	this->settingToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
	this->faceToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
	this->pickToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
	this->deleteToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
	this->vertexToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
	this->toolStripContainer2 = (gcnew System::Windows::Forms::ToolStripContainer());
	this->groupLayoutPannel = (gcnew System::Windows::Forms::FlowLayoutPanel());
	this->addGroupButton = (gcnew System::Windows::Forms::Button());
	this->backgroundWorker1 = (gcnew System::ComponentModel::BackgroundWorker());
	this->menuStrip1->SuspendLayout();
	this->toolStripContainer2->ContentPanel->SuspendLayout();
	this->toolStripContainer2->TopToolStripPanel->SuspendLayout();
	this->toolStripContainer2->SuspendLayout();
	this->SuspendLayout();
	// 
	// timer1
	// 
	this->timer1->Enabled = true;
	this->timer1->Interval = 16;
	this->timer1->Tick += gcnew System::EventHandler(this, &Form1::timer1_Tick);
	// 
	// hkoglPanelControl1
	// 
	this->hkoglPanelControl1->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Left | System::Windows::Forms::AnchorStyles::Right));
	this->hkoglPanelControl1->AutoSize = true;
	hkcoglPanelCameraSetting2->Far = 1000;
	hkcoglPanelCameraSetting2->Fov = 45;
	hkcoglPanelCameraSetting2->Near = -1000;
	hkcoglPanelCameraSetting2->Type = HKOGLPanel::HKCOGLPanelCameraSetting::CAMERATYPE::ORTHOGRAPHIC;
	this->hkoglPanelControl1->Camera_Setting = hkcoglPanelCameraSetting2;
	this->hkoglPanelControl1->Location = System::Drawing::Point(3, 28);
	this->hkoglPanelControl1->Name = L"hkoglPanelControl1";
	hkcoglPanelPixelFormat2->Accumu_Buffer_Bits = HKOGLPanel::HKCOGLPanelPixelFormat::PIXELBITS::BITS_0;
	hkcoglPanelPixelFormat2->Alpha_Buffer_Bits = HKOGLPanel::HKCOGLPanelPixelFormat::PIXELBITS::BITS_0;
	hkcoglPanelPixelFormat2->Stencil_Buffer_Bits = HKOGLPanel::HKCOGLPanelPixelFormat::PIXELBITS::BITS_0;
	this->hkoglPanelControl1->Pixel_Format = hkcoglPanelPixelFormat2;
	this->hkoglPanelControl1->Size = System::Drawing::Size(800, 600);
	this->hkoglPanelControl1->TabIndex = 0;
	this->hkoglPanelControl1->Load += gcnew System::EventHandler(this, &Form1::hkoglPanelControl1_Load);
	this->hkoglPanelControl1->Paint += gcnew System::Windows::Forms::PaintEventHandler(this, &Form1::hkoglPanelControl1_Paint);
	this->hkoglPanelControl1->KeyPress += gcnew System::Windows::Forms::KeyPressEventHandler(this, &Form1::hkoglPanelControl1_KeyPress);
	this->hkoglPanelControl1->MouseDown += gcnew System::Windows::Forms::MouseEventHandler(this, &Form1::hkoglPanelControl1_MouseDown);
	this->hkoglPanelControl1->MouseMove += gcnew System::Windows::Forms::MouseEventHandler(this, &Form1::hkoglPanelControl1_MouseMove);
	this->hkoglPanelControl1->MouseUp += gcnew System::Windows::Forms::MouseEventHandler(this, &Form1::hkoglPanelControl1_MouseUp);
	this->hkoglPanelControl1->MouseWheel += gcnew System::Windows::Forms::MouseEventHandler(this, &Form1::hkoglPanelControl1_MouseWheel);
	// 
	// menuStrip1
	// 
	this->menuStrip1->Dock = System::Windows::Forms::DockStyle::None;
	this->menuStrip1->Items->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(1) { this->settingToolStripMenuItem });
	this->menuStrip1->Location = System::Drawing::Point(0, 0);
	this->menuStrip1->Name = L"menuStrip1";
	this->menuStrip1->Size = System::Drawing::Size(1264, 24);
	this->menuStrip1->TabIndex = 1;
	this->menuStrip1->Text = L"menuStrip1";
	// 
	// settingToolStripMenuItem
	// 
	this->settingToolStripMenuItem->DropDownItems->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(2) {
		this->faceToolStripMenuItem,
			this->vertexToolStripMenuItem
	});
	this->settingToolStripMenuItem->Name = L"settingToolStripMenuItem";
	this->settingToolStripMenuItem->Size = System::Drawing::Size(59, 20);
	this->settingToolStripMenuItem->Text = L"Setting";
	// 
	// faceToolStripMenuItem
	// 
	this->faceToolStripMenuItem->DropDownItems->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(2) {
		this->pickToolStripMenuItem,
			this->deleteToolStripMenuItem
	});
	this->faceToolStripMenuItem->Name = L"faceToolStripMenuItem";
	this->faceToolStripMenuItem->Size = System::Drawing::Size(132, 22);
	this->faceToolStripMenuItem->Text = L"Face";
	// 
	// pickToolStripMenuItem
	// 
	this->pickToolStripMenuItem->Name = L"pickToolStripMenuItem";
	this->pickToolStripMenuItem->Size = System::Drawing::Size(111, 22);
	this->pickToolStripMenuItem->Text = L"Pick";
	this->pickToolStripMenuItem->Click += gcnew System::EventHandler(this, &Form1::pickToolStripMenuItem_Click);
	// 
	// deleteToolStripMenuItem
	// 
	this->deleteToolStripMenuItem->Name = L"deleteToolStripMenuItem";
	this->deleteToolStripMenuItem->Size = System::Drawing::Size(111, 22);
	this->deleteToolStripMenuItem->Text = L"Delete";
	this->deleteToolStripMenuItem->Click += gcnew System::EventHandler(this, &Form1::deleteToolStripMenuItem_Click);
	// 
	// vertexToolStripMenuItem
	// 
	this->vertexToolStripMenuItem->Name = L"vertexToolStripMenuItem";
	this->vertexToolStripMenuItem->Size = System::Drawing::Size(132, 22);
	this->vertexToolStripMenuItem->Text = L"PickVertex";
	this->vertexToolStripMenuItem->Click += gcnew System::EventHandler(this, &Form1::vertexToolStripMenuItem_Click);
	// 
	// toolStripContainer2
	// 
	this->toolStripContainer2->BottomToolStripPanelVisible = false;
	// 
	// toolStripContainer2.ContentPanel
	// 
	this->toolStripContainer2->ContentPanel->Controls->Add(this->groupLayoutPannel);
	this->toolStripContainer2->ContentPanel->Controls->Add(this->addGroupButton);
	this->toolStripContainer2->ContentPanel->Controls->Add(this->hkoglPanelControl1);
	this->toolStripContainer2->ContentPanel->Margin = System::Windows::Forms::Padding(4, 5, 4, 5);
	this->toolStripContainer2->ContentPanel->Size = System::Drawing::Size(1264, 657);
	this->toolStripContainer2->Dock = System::Windows::Forms::DockStyle::Fill;
	this->toolStripContainer2->LeftToolStripPanelVisible = false;
	this->toolStripContainer2->Location = System::Drawing::Point(0, 0);
	this->toolStripContainer2->Margin = System::Windows::Forms::Padding(4, 5, 4, 5);
	this->toolStripContainer2->Name = L"toolStripContainer2";
	this->toolStripContainer2->RightToolStripPanelVisible = false;
	this->toolStripContainer2->Size = System::Drawing::Size(1264, 681);
	this->toolStripContainer2->TabIndex = 2;
	this->toolStripContainer2->Text = L"toolStripContainer2";
	// 
	// toolStripContainer2.TopToolStripPanel
	// 
	this->toolStripContainer2->TopToolStripPanel->Controls->Add(this->menuStrip1);
	// 
	// groupLayoutPannel
	// 
	this->groupLayoutPannel->AutoScroll = true;
	this->groupLayoutPannel->Location = System::Drawing::Point(806, 28);
	this->groupLayoutPannel->Name = L"groupLayoutPannel";
	this->groupLayoutPannel->Size = System::Drawing::Size(455, 563);
	this->groupLayoutPannel->TabIndex = 3;
	// 
	// addGroupButton
	// 
	this->addGroupButton->Location = System::Drawing::Point(806, 596);
	this->addGroupButton->Name = L"addGroupButton";
	this->addGroupButton->Size = System::Drawing::Size(455, 32);
	this->addGroupButton->TabIndex = 3;
	this->addGroupButton->Text = L"Add Group";
	this->addGroupButton->UseVisualStyleBackColor = true;
	this->addGroupButton->Click += gcnew System::EventHandler(this, &Form1::onAddGroupButtonClick);
	// 
	// Form1
	// 
	this->AutoScaleDimensions = System::Drawing::SizeF(10, 20);
	this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
	this->ClientSize = System::Drawing::Size(1264, 681);
	this->Controls->Add(this->toolStripContainer2);
	this->Font = (gcnew System::Drawing::Font(L"·L³n¥¿¶ÂÅé", 12, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
		static_cast<System::Byte>(136)));
	this->MainMenuStrip = this->menuStrip1;
	this->Margin = System::Windows::Forms::Padding(3, 4, 3, 4);
	this->Name = L"Form1";
	this->Text = L"MyForm";
	this->menuStrip1->ResumeLayout(false);
	this->menuStrip1->PerformLayout();
	this->toolStripContainer2->ContentPanel->ResumeLayout(false);
	this->toolStripContainer2->ContentPanel->PerformLayout();
	this->toolStripContainer2->TopToolStripPanel->ResumeLayout(false);
	this->toolStripContainer2->TopToolStripPanel->PerformLayout();
	this->toolStripContainer2->ResumeLayout(false);
	this->toolStripContainer2->PerformLayout();
	this->ResumeLayout(false);

}

System::Void CppCLRWinformsProject::Form1::hkoglPanelControl1_Load(System::Object^ sender, System::EventArgs^ e)
{
	GLenum res = glewInit();
	if (res != GLEW_OK) {
		fprintf(stderr, "Error: '%s'\n", glewGetErrorString(res));
	}

	int ver[2] = { 0, 0 };
	glGetIntegerv(GL_MAJOR_VERSION, &ver[0]);
	glGetIntegerv(GL_MINOR_VERSION, &ver[1]);
	printf("OpenGL Version: %d.%d\n", ver[0], ver[1]);

	scene = new Scene;

	scene->draw();
}

System::Void CppCLRWinformsProject::Form1::hkoglPanelControl1_Paint(System::Object^ sender, System::Windows::Forms::PaintEventArgs^ e)
{
	clock_t currentFrame = clock();
	deltaTime = currentFrame - lastFrame;
	lastFrame = currentFrame;

	scene->draw();
}

System::Void CppCLRWinformsProject::Form1::timer1_Tick(System::Object^ sender, System::EventArgs^ e)
{
	hkoglPanelControl1->Invalidate();
}

System::Void CppCLRWinformsProject::Form1::hkoglPanelControl1_MouseMove(System::Object^ sender, System::Windows::Forms::MouseEventArgs^ e)
{
	if (e->Button.ToString() == "Left")
		scene->picking(e->X, e->Y);
	else if (e->Button.ToString() == "Right")
		scene->changeDirection(e->X, e->Y);
	if (e->Button.ToString() == "Middle")
		scene->changePosition(e->X, e->Y);
		//printf("Middle\n");
	hkoglPanelControl1->Invalidate();
}

System::Void CppCLRWinformsProject::Form1::hkoglPanelControl1_MouseUp(System::Object^ sender, System::Windows::Forms::MouseEventArgs^ e)
{
	scene->mouseUp();
}

System::Void CppCLRWinformsProject::Form1::hkoglPanelControl1_KeyPress(System::Object^ sender, System::Windows::Forms::KeyPressEventArgs^ e)
{
	scene->changePosition(e->KeyChar, deltaTime);
	hkoglPanelControl1->Invalidate();
}

System::Void CppCLRWinformsProject::Form1::hkoglPanelControl1_MouseDown(System::Object^ sender, System::Windows::Forms::MouseEventArgs^ e) 
{
	if (e->Button.ToString() == "Left")
		scene->picking(e->X, e->Y);
}

System::Void CppCLRWinformsProject::Form1::hkoglPanelControl1_MouseWheel(System::Object^ sender, System::Windows::Forms::MouseEventArgs^ e)
{
	scene->changeDistance(e->Delta);
}

System::Void CppCLRWinformsProject::Form1::pickToolStripMenuItem_Click(System::Object^ sender, System::EventArgs^ e)
{
	scene->mode = 0;
}

System::Void CppCLRWinformsProject::Form1::deleteToolStripMenuItem_Click(System::Object^ sender, System::EventArgs^ e)
{
	scene->mode = 1;
}

System::Void CppCLRWinformsProject::Form1::vertexToolStripMenuItem_Click(System::Object^ sender, System::EventArgs^ e)
{
	scene->mode = 2;
}

void CppCLRWinformsProject::Form1::onAddGroupButtonClick(System::Object^ sender, System::EventArgs^ e)
{
	GroupBox^ newGroupBox = gcnew GroupBox();
	Button^ newButton = gcnew Button();

	// Init button
	newButton->Location = System::Drawing::Point(8, 43);
	newButton->Name = L"button" + groupBoxesList->Count;
	newButton->Size = System::Drawing::Size(79, 28);
	newButton->TabIndex = 0;
	newButton->Text = L"Temp";
	newButton->UseVisualStyleBackColor = true;
	newButton->Click += gcnew System::EventHandler(this, &CppCLRWinformsProject::Form1::onGroupDeleteButtonClick);

	// Assigned button to group box
	newGroupBox->Controls->Add(newButton);

	// Input box, used for get group name
	Form^ inputBox = gcnew Form();
	inputBox->Size = System::Drawing::Size(400, 300);
	inputBox->Text = L"Input Box";
	inputBox->Font = (gcnew System::Drawing::Font(L"·L³n¥¿¶ÂÅé", 12, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
		static_cast<System::Byte>(136)));

	Label^ label = gcnew Label();
	label->Text = L"Enter the group name";
	label->Size = System::Drawing::Size(300, 50);
	label->Location = System::Drawing::Point(50, 50);

	TextBox^ inputField = gcnew TextBox();
	inputField->Size = System::Drawing::Size(300, 50);
	inputField->Location = System::Drawing::Point(50, 110);

	Button^ okButton = gcnew Button();
	okButton->Size = System::Drawing::Size(50, 25);
	okButton->Location = System::Drawing::Point(50, 165);
	okButton->Text = L"OK!";

	inputBox->Controls->Add(label);
	inputBox->Controls->Add(inputField);
	inputBox->Controls->Add(okButton);
	inputBox->ShowDialog();


	String^ groupName = inputField->Text;

	// Init group box
	newGroupBox->Size = System::Drawing::Size(429, 81);
	newGroupBox->Margin = System::Windows::Forms::Padding(4, 5, 4, 5);
	newGroupBox->Name = groupName;
	newGroupBox->Padding = System::Windows::Forms::Padding(4, 5, 4, 5);
	newGroupBox->TabIndex = 3;
	newGroupBox->TabStop = false;
	newGroupBox->Text = groupName;

	// Assigned to layout pannel
	this->groupLayoutPannel->Controls->Add(newGroupBox);

	// Assigned to list
	this->groupBoxesList->Add(newGroupBox);
}


System::Void CppCLRWinformsProject::Form1::onGroupDeleteButtonClick(System::Object^ sender, System::EventArgs^ e)
{
	Button^ button = safe_cast<Button^>(sender);
	GroupBox^ groupBox = safe_cast<GroupBox^>(button->Parent);

	button->Click -= gcnew System::EventHandler(this, &CppCLRWinformsProject::Form1::onGroupDeleteButtonClick);

	groupBoxesList->Remove(groupBox);
	groupBox->Controls->Remove(button);
	groupLayoutPannel->Controls->Remove(groupBox);
}
