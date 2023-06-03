#include "pch.h"
#include "Form1.h"

// #include "Form1.h"
#define GLEW_STATIC
#include <GL/glew.h>
#include <iostream>

CppCLRWinformsProject::Form1::Form1(void)
{
	InitializeComponent();
	graph = this->panel1->CreateGraphics();
	pen = gcnew Pen(Color::Blue, 3);
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
	HKOGLPanel::HKCOGLPanelCameraSetting^ hkcoglPanelCameraSetting1 = (gcnew HKOGLPanel::HKCOGLPanelCameraSetting());
	HKOGLPanel::HKCOGLPanelPixelFormat^ hkcoglPanelPixelFormat1 = (gcnew HKOGLPanel::HKCOGLPanelPixelFormat());
	this->timer1 = (gcnew System::Windows::Forms::Timer(this->components));
	this->hkoglPanelControl1 = (gcnew HKOGLPanel::HKOGLPanelControl());
	this->menuStrip1 = (gcnew System::Windows::Forms::MenuStrip());
	this->settingToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
	this->faceToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
	this->pickToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
	this->deleteToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
	this->vertexToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
	this->toolStripContainer2 = (gcnew System::Windows::Forms::ToolStripContainer());
	this->panel1 = (gcnew System::Windows::Forms::Panel());
	this->testKeyToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
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
	this->hkoglPanelControl1->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Bottom)
		| System::Windows::Forms::AnchorStyles::Left)
		| System::Windows::Forms::AnchorStyles::Right));
	this->hkoglPanelControl1->AutoSize = true;
	hkcoglPanelCameraSetting1->Far = 1000;
	hkcoglPanelCameraSetting1->Fov = 45;
	hkcoglPanelCameraSetting1->Near = -1000;
	hkcoglPanelCameraSetting1->Type = HKOGLPanel::HKCOGLPanelCameraSetting::CAMERATYPE::ORTHOGRAPHIC;
	this->hkoglPanelControl1->Camera_Setting = hkcoglPanelCameraSetting1;
	this->hkoglPanelControl1->Location = System::Drawing::Point(2, 2);
	this->hkoglPanelControl1->Margin = System::Windows::Forms::Padding(2);
	this->hkoglPanelControl1->Name = L"hkoglPanelControl1";
	hkcoglPanelPixelFormat1->Accumu_Buffer_Bits = HKOGLPanel::HKCOGLPanelPixelFormat::PIXELBITS::BITS_0;
	hkcoglPanelPixelFormat1->Alpha_Buffer_Bits = HKOGLPanel::HKCOGLPanelPixelFormat::PIXELBITS::BITS_0;
	hkcoglPanelPixelFormat1->Stencil_Buffer_Bits = HKOGLPanel::HKCOGLPanelPixelFormat::PIXELBITS::BITS_0;
	this->hkoglPanelControl1->Pixel_Format = hkcoglPanelPixelFormat1;
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
	this->menuStrip1->Size = System::Drawing::Size(1188, 24);
	this->menuStrip1->TabIndex = 1;
	this->menuStrip1->Text = L"menuStrip1";
	// 
	// settingToolStripMenuItem
	// 
	this->settingToolStripMenuItem->DropDownItems->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(3) {
		this->faceToolStripMenuItem,
			this->vertexToolStripMenuItem, this->testKeyToolStripMenuItem
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
	this->faceToolStripMenuItem->Size = System::Drawing::Size(180, 22);
	this->faceToolStripMenuItem->Text = L"Face";
	// 
	// pickToolStripMenuItem
	// 
	this->pickToolStripMenuItem->Name = L"pickToolStripMenuItem";
	this->pickToolStripMenuItem->Size = System::Drawing::Size(180, 22);
	this->pickToolStripMenuItem->Text = L"Pick";
	this->pickToolStripMenuItem->Click += gcnew System::EventHandler(this, &Form1::pickToolStripMenuItem_Click);
	// 
	// deleteToolStripMenuItem
	// 
	this->deleteToolStripMenuItem->Name = L"deleteToolStripMenuItem";
	this->deleteToolStripMenuItem->Size = System::Drawing::Size(180, 22);
	this->deleteToolStripMenuItem->Text = L"Delete";
	this->deleteToolStripMenuItem->Click += gcnew System::EventHandler(this, &Form1::deleteToolStripMenuItem_Click);
	// 
	// vertexToolStripMenuItem
	// 
	this->vertexToolStripMenuItem->Name = L"vertexToolStripMenuItem";
	this->vertexToolStripMenuItem->Size = System::Drawing::Size(180, 22);
	this->vertexToolStripMenuItem->Text = L"PickVertex";
	this->vertexToolStripMenuItem->Click += gcnew System::EventHandler(this, &Form1::vertexToolStripMenuItem_Click);
	// 
	// toolStripContainer2
	// 
	this->toolStripContainer2->BottomToolStripPanelVisible = false;
	// 
	// toolStripContainer2.ContentPanel
	// 
	this->toolStripContainer2->ContentPanel->Controls->Add(this->panel1);
	this->toolStripContainer2->ContentPanel->Controls->Add(this->hkoglPanelControl1);
	this->toolStripContainer2->ContentPanel->Size = System::Drawing::Size(1188, 604);
	this->toolStripContainer2->Dock = System::Windows::Forms::DockStyle::Fill;
	this->toolStripContainer2->LeftToolStripPanelVisible = false;
	this->toolStripContainer2->Location = System::Drawing::Point(0, 0);
	this->toolStripContainer2->Name = L"toolStripContainer2";
	this->toolStripContainer2->RightToolStripPanelVisible = false;
	this->toolStripContainer2->Size = System::Drawing::Size(1188, 628);
	this->toolStripContainer2->TabIndex = 2;
	this->toolStripContainer2->Text = L"toolStripContainer2";
	// 
	// toolStripContainer2.TopToolStripPanel
	// 
	this->toolStripContainer2->TopToolStripPanel->Controls->Add(this->menuStrip1);
	// 
	// panel1
	// 
	this->panel1->BackColor = System::Drawing::Color::White;
	this->panel1->Location = System::Drawing::Point(886, 103);
	this->panel1->Name = L"panel1";
	this->panel1->Size = System::Drawing::Size(240, 240);
	this->panel1->TabIndex = 1;
	// 
	// testKeyToolStripMenuItem
	// 
	this->testKeyToolStripMenuItem->Name = L"testKeyToolStripMenuItem";
	this->testKeyToolStripMenuItem->Size = System::Drawing::Size(180, 22);
	this->testKeyToolStripMenuItem->Text = L"TestKey";
	this->testKeyToolStripMenuItem->Click += gcnew System::EventHandler(this, &Form1::testKeyToolStripMenuItem_Click);
	// 
	// Form1
	// 
	this->AutoScaleDimensions = System::Drawing::SizeF(6, 12);
	this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
	this->ClientSize = System::Drawing::Size(1188, 628);
	this->Controls->Add(this->toolStripContainer2);
	this->MainMenuStrip = this->menuStrip1;
	this->Margin = System::Windows::Forms::Padding(2);
	this->Name = L"Form1";
	this->Text = L"Form1";
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
	if (e->Button.ToString() == "Right")
		scene->changeDirection(e->X, e->Y);
	else if (e->Button.ToString() == "Middle")
		scene->changePosition(e->X, e->Y);
	else if (e->Button.ToString() == "Left")
		scene->picking(e->X, e->Y);


	if (e->Button.ToString() == "Left")
	{
		graph->Clear(Color::White);

		std::vector<float> lengths;
		scene->calculateSurround(lengths);
		if (lengths.size() == 0)return;

		PointF prev_pointf = transCoord(lengths[0]);

		for (auto it = lengths.begin() + 1; it != lengths.end(); it++)
		{
			PointF pointf = transCoord(*it);
			if (pen->Color == Color::Blue) pen->Color = Color::Red;
			else pen->Color = Color::Blue;
			graph->DrawLine(pen, prev_pointf, pointf);
			prev_pointf = pointf;
		}
		if (pen->Color == Color::Blue) pen->Color = Color::Red;
		else pen->Color = Color::Blue;
		graph->DrawLine(pen, prev_pointf, transCoord(lengths[0]));
	}

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

	if (e->Button.ToString() == "Left")
	{
		graph->Clear(Color::White);

		std::vector<float> lengths;
		scene->calculateSurround(lengths);
		if (lengths.size() == 0)return;

		PointF prev_pointf = transCoord(lengths[0]);

		for (auto it = lengths.begin() + 1; it != lengths.end(); it++)
		{
			PointF pointf = transCoord(*it);
			if (pen->Color == Color::Blue) pen->Color = Color::Red;
			else pen->Color = Color::Blue;
			graph->DrawLine(pen, prev_pointf, pointf);
			prev_pointf = pointf;
		}
		if (pen->Color == Color::Blue) pen->Color = Color::Red;
		else pen->Color = Color::Blue;
		graph->DrawLine(pen, prev_pointf, transCoord(lengths[0]));
	}
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

System::Void CppCLRWinformsProject::Form1::testKeyToolStripMenuItem_Click(System::Object^ sender, System::EventArgs^ e)
{

}

System::Drawing::PointF CppCLRWinformsProject::Form1::transCoord(float length)
{
	if (length >= 0.0f && length < 0.25f)
	{
		return PointF(40.0f + 160.0f * length * 4.0f, 200.0f);
	}
	else if (length >= 0.25f && length < 0.5f)
	{
		return PointF(200.0f, 200.0f - 160.0f * (length - 0.25f) * 4);
	}
	else if (length >= 0.5f && length < 0.75f)
	{
		return PointF(200.0f - 160.0f * (length - 0.5f) * 4, 40.0f);
	}
	else if (length >= 0.75f)
	{
		return PointF(40.0f, 40.0f + 160.0f * (length - 0.75f) * 4);
	}
	return PointF();
}
