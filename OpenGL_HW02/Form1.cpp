#include "pch.h"
#include "Form1.h"

/*                 Standard                 */
#include <iostream>
#include <ctime>


/*                 OpenGL                   */
#define GLEW_STATIC
#include <GL/glew.h>


/*                 GLM                      */
#include <glm/glm.hpp>


/*                 stb                      */
#define STB_IMAGE_IMPLEMENTATION
#include <STB/stb_image.h>


/*                 My Class                 */
#include "Scene.h"
#include "Config.h"
#include "utilities.h"
#include "TextureParamPainter.h"
#include "ConvexCombMap.h"


CppCLRWinformsProject::Form1::Form1(void)
{
	InitializeComponent();

	lastFrame = clock();
}

CppCLRWinformsProject::Form1::~Form1()
{
	delete scene;
	if (components)
	{
		delete components;
	}
}

std::vector<glm::vec2> CppCLRWinformsProject::Form1::getTextureParamVertices()
{
	const TriMesh& mesh = scene->getSelectedMeshRef();

	std::vector<glm::vec2> textureCoords;

	for (TriMesh::HalfedgeIter he_it = mesh.halfedges_begin(); he_it != mesh.halfedges_end(); ++he_it)
	{
		glm::vec3 v1 = Utils::toGlmVec3(mesh.point(mesh.to_vertex_handle(*he_it)));
		glm::vec3 v2 = Utils::toGlmVec3(mesh.point(mesh.from_vertex_handle(*he_it)));

		try
		{
			glm::vec2 uv1 = ConvexCombMap::map(v1);
			glm::vec2 uv2 = ConvexCombMap::map(v2);

			uv1.x = uv1.x * 2 - 1;
			uv1.y = uv1.y * 2 - 1;

			uv2.x = uv2.x * 2 - 1;
			uv2.y = uv2.y * 2 - 1;

			textureCoords.push_back(uv1);
			textureCoords.push_back(uv2);
		}
		catch (std::out_of_range e)
		{
			return {};
		}
	}

	return textureCoords;
}

/* WindowsForm Initializer. (Don't manully modify this function)*/
void CppCLRWinformsProject::Form1::InitializeComponent(void)
{
	HKOGLPanel::HKCOGLPanelCameraSetting^ hkcoglPanelCameraSetting1 = (gcnew HKOGLPanel::HKCOGLPanelCameraSetting());
	HKOGLPanel::HKCOGLPanelPixelFormat^ hkcoglPanelPixelFormat1 = (gcnew HKOGLPanel::HKCOGLPanelPixelFormat());
	HKOGLPanel::HKCOGLPanelCameraSetting^ hkcoglPanelCameraSetting2 = (gcnew HKOGLPanel::HKCOGLPanelCameraSetting());
	HKOGLPanel::HKCOGLPanelPixelFormat^ hkcoglPanelPixelFormat2 = (gcnew HKOGLPanel::HKCOGLPanelPixelFormat());
	this->menuStrip1 = (gcnew System::Windows::Forms::MenuStrip());
	this->settingToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
	this->faceToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
	this->vertexToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
	this->pickToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
	this->deleteToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
	this->sobelFilterKeyToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
	this->gridToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
	this->fileToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
	this->saveToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
	this->loadToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
	this->scaleTextbox = (gcnew System::Windows::Forms::TextBox());
	this->rotateTextBox = (gcnew System::Windows::Forms::TextBox());
	this->newSelectButton = (gcnew System::Windows::Forms::Button());
	this->loadTexButton = (gcnew System::Windows::Forms::Button());
	this->scaleButton = (gcnew System::Windows::Forms::Button());
	this->rotateButton = (gcnew System::Windows::Forms::Button());
	this->calculateButton = (gcnew System::Windows::Forms::Button());
	this->hkoglPanelControl2 = (gcnew HKOGLPanel::HKOGLPanelControl());
	this->openFileDialog1 = (gcnew System::Windows::Forms::OpenFileDialog());
	this->BottomToolStripPanel = (gcnew System::Windows::Forms::ToolStripPanel());
	this->TopToolStripPanel = (gcnew System::Windows::Forms::ToolStripPanel());
	this->RightToolStripPanel = (gcnew System::Windows::Forms::ToolStripPanel());
	this->LeftToolStripPanel = (gcnew System::Windows::Forms::ToolStripPanel());
	this->ContentPanel = (gcnew System::Windows::Forms::ToolStripContentPanel());
	this->hkoglPanelControl1 = (gcnew HKOGLPanel::HKOGLPanelControl());
	this->tableLayoutPanel1 = (gcnew System::Windows::Forms::TableLayoutPanel());
	this->tableLayoutPanel2 = (gcnew System::Windows::Forms::TableLayoutPanel());
	this->splitContainer2 = (gcnew System::Windows::Forms::SplitContainer());
	this->splitContainer3 = (gcnew System::Windows::Forms::SplitContainer());
	this->tableLayoutPanel3 = (gcnew System::Windows::Forms::TableLayoutPanel());
	this->menuStrip1->SuspendLayout();
	this->tableLayoutPanel1->SuspendLayout();
	this->tableLayoutPanel2->SuspendLayout();
	(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->splitContainer2))->BeginInit();
	this->splitContainer2->Panel1->SuspendLayout();
	this->splitContainer2->Panel2->SuspendLayout();
	this->splitContainer2->SuspendLayout();
	(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->splitContainer3))->BeginInit();
	this->splitContainer3->Panel1->SuspendLayout();
	this->splitContainer3->Panel2->SuspendLayout();
	this->splitContainer3->SuspendLayout();
	this->tableLayoutPanel3->SuspendLayout();
	this->SuspendLayout();
	// 
	// menuStrip1
	// 
	this->menuStrip1->Dock = System::Windows::Forms::DockStyle::Fill;
	this->menuStrip1->Items->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(2) {
		this->settingToolStripMenuItem,
			this->fileToolStripMenuItem
	});
	this->menuStrip1->Location = System::Drawing::Point(0, 0);
	this->menuStrip1->Name = L"menuStrip1";
	this->menuStrip1->Size = System::Drawing::Size(1264, 25);
	this->menuStrip1->TabIndex = 1;
	this->menuStrip1->Text = L"menuStrip1";
	// 
	// settingToolStripMenuItem
	// 
	this->settingToolStripMenuItem->DropDownItems->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(3) {
		this->faceToolStripMenuItem,
			this->sobelFilterKeyToolStripMenuItem, this->gridToolStripMenuItem
	});
	this->settingToolStripMenuItem->Name = L"settingToolStripMenuItem";
	this->settingToolStripMenuItem->Size = System::Drawing::Size(59, 21);
	this->settingToolStripMenuItem->Text = L"Setting";
	// 
	// faceToolStripMenuItem
	// 
	this->faceToolStripMenuItem->DropDownItems->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(3) {
		this->vertexToolStripMenuItem,
			this->pickToolStripMenuItem, this->deleteToolStripMenuItem
	});
	this->faceToolStripMenuItem->Name = L"faceToolStripMenuItem";
	this->faceToolStripMenuItem->Size = System::Drawing::Size(180, 22);
	this->faceToolStripMenuItem->Text = L"Face";
	// 
	// vertexToolStripMenuItem
	// 
	this->vertexToolStripMenuItem->Name = L"vertexToolStripMenuItem";
	this->vertexToolStripMenuItem->Size = System::Drawing::Size(132, 22);
	this->vertexToolStripMenuItem->Text = L"PickVertex";
	this->vertexToolStripMenuItem->Click += gcnew System::EventHandler(this, &Form1::vertexToolStripMenuItem_Click);
	// 
	// pickToolStripMenuItem
	// 
	this->pickToolStripMenuItem->Name = L"pickToolStripMenuItem";
	this->pickToolStripMenuItem->Size = System::Drawing::Size(132, 22);
	this->pickToolStripMenuItem->Text = L"Pick";
	this->pickToolStripMenuItem->Click += gcnew System::EventHandler(this, &Form1::pickToolStripMenuItem_Click);
	// 
	// deleteToolStripMenuItem
	// 
	this->deleteToolStripMenuItem->Name = L"deleteToolStripMenuItem";
	this->deleteToolStripMenuItem->Size = System::Drawing::Size(132, 22);
	this->deleteToolStripMenuItem->Text = L"Delete";
	this->deleteToolStripMenuItem->Click += gcnew System::EventHandler(this, &Form1::deleteToolStripMenuItem_Click);
	// 
	// sobelFilterKeyToolStripMenuItem
	// 
	this->sobelFilterKeyToolStripMenuItem->Name = L"sobelFilterKeyToolStripMenuItem";
	this->sobelFilterKeyToolStripMenuItem->Size = System::Drawing::Size(180, 22);
	this->sobelFilterKeyToolStripMenuItem->Text = L"Sobel Filter";
	this->sobelFilterKeyToolStripMenuItem->Click += gcnew System::EventHandler(this, &Form1::sobelFilterKeyToolStripMenuItem_Click);
	// 
	// gridToolStripMenuItem
	// 
	this->gridToolStripMenuItem->Name = L"gridToolStripMenuItem";
	this->gridToolStripMenuItem->Size = System::Drawing::Size(180, 22);
	this->gridToolStripMenuItem->Text = L"Grid";
	this->gridToolStripMenuItem->Click += gcnew System::EventHandler(this, &Form1::gridToolStripMenuItem_Click);
	// 
	// fileToolStripMenuItem
	// 
	this->fileToolStripMenuItem->DropDownItems->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(2) {
		this->saveToolStripMenuItem,
			this->loadToolStripMenuItem
	});
	this->fileToolStripMenuItem->Name = L"fileToolStripMenuItem";
	this->fileToolStripMenuItem->Size = System::Drawing::Size(38, 21);
	this->fileToolStripMenuItem->Text = L"File";
	// 
	// saveToolStripMenuItem
	// 
	this->saveToolStripMenuItem->Name = L"saveToolStripMenuItem";
	this->saveToolStripMenuItem->Size = System::Drawing::Size(180, 22);
	this->saveToolStripMenuItem->Text = L"Save";
	this->saveToolStripMenuItem->Click += gcnew System::EventHandler(this, &Form1::saveToolStripMenuItem_Click);
	// 
	// loadToolStripMenuItem
	// 
	this->loadToolStripMenuItem->Name = L"loadToolStripMenuItem";
	this->loadToolStripMenuItem->Size = System::Drawing::Size(180, 22);
	this->loadToolStripMenuItem->Text = L"Load";
	this->loadToolStripMenuItem->Click += gcnew System::EventHandler(this, &Form1::loadToolStripMenuItem_Click);
	// 
	// scaleTextbox
	// 
	this->scaleTextbox->Dock = System::Windows::Forms::DockStyle::Bottom;
	this->scaleTextbox->Location = System::Drawing::Point(0, 2);
	this->scaleTextbox->Name = L"scaleTextbox";
	this->scaleTextbox->Size = System::Drawing::Size(291, 22);
	this->scaleTextbox->TabIndex = 8;
	// 
	// rotateTextBox
	// 
	this->rotateTextBox->Dock = System::Windows::Forms::DockStyle::Bottom;
	this->rotateTextBox->Location = System::Drawing::Point(0, 2);
	this->rotateTextBox->Name = L"rotateTextBox";
	this->rotateTextBox->Size = System::Drawing::Size(291, 22);
	this->rotateTextBox->TabIndex = 7;
	// 
	// newSelectButton
	// 
	this->newSelectButton->Dock = System::Windows::Forms::DockStyle::Fill;
	this->newSelectButton->Font = (gcnew System::Drawing::Font(L"Microsoft JhengHei", 12));
	this->newSelectButton->Location = System::Drawing::Point(3, 560);
	this->newSelectButton->Name = L"newSelectButton";
	this->newSelectButton->Size = System::Drawing::Size(442, 37);
	this->newSelectButton->TabIndex = 6;
	this->newSelectButton->Text = L"Select new mesh";
	this->newSelectButton->UseVisualStyleBackColor = true;
	this->newSelectButton->Click += gcnew System::EventHandler(this, &Form1::newSelectButton_Click);
	// 
	// loadTexButton
	// 
	this->loadTexButton->Dock = System::Windows::Forms::DockStyle::Fill;
	this->loadTexButton->Font = (gcnew System::Drawing::Font(L"Microsoft JhengHei", 12));
	this->loadTexButton->Location = System::Drawing::Point(3, 517);
	this->loadTexButton->Name = L"loadTexButton";
	this->loadTexButton->Size = System::Drawing::Size(442, 37);
	this->loadTexButton->TabIndex = 5;
	this->loadTexButton->Text = L"Load Texture";
	this->loadTexButton->UseVisualStyleBackColor = true;
	this->loadTexButton->Click += gcnew System::EventHandler(this, &Form1::loadTexButton_Click);
	// 
	// scaleButton
	// 
	this->scaleButton->Dock = System::Windows::Forms::DockStyle::Fill;
	this->scaleButton->Font = (gcnew System::Drawing::Font(L"Microsoft JhengHei", 12));
	this->scaleButton->Location = System::Drawing::Point(0, 0);
	this->scaleButton->Name = L"scaleButton";
	this->scaleButton->Size = System::Drawing::Size(147, 24);
	this->scaleButton->TabIndex = 4;
	this->scaleButton->Text = L"Scale ";
	this->scaleButton->UseVisualStyleBackColor = true;
	this->scaleButton->Click += gcnew System::EventHandler(this, &Form1::scaleButton_Click);
	// 
	// rotateButton
	// 
	this->rotateButton->Dock = System::Windows::Forms::DockStyle::Fill;
	this->rotateButton->Font = (gcnew System::Drawing::Font(L"Microsoft JhengHei", 12, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
		static_cast<System::Byte>(136)));
	this->rotateButton->Location = System::Drawing::Point(0, 0);
	this->rotateButton->Name = L"rotateButton";
	this->rotateButton->Size = System::Drawing::Size(147, 24);
	this->rotateButton->TabIndex = 3;
	this->rotateButton->Text = L"Rotate";
	this->rotateButton->UseVisualStyleBackColor = true;
	this->rotateButton->Click += gcnew System::EventHandler(this, &Form1::rotateButton_Click);
	// 
	// calculateButton
	// 
	this->calculateButton->Dock = System::Windows::Forms::DockStyle::Fill;
	this->calculateButton->Font = (gcnew System::Drawing::Font(L"Microsoft JhengHei", 12, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
		static_cast<System::Byte>(136)));
	this->calculateButton->Location = System::Drawing::Point(3, 3);
	this->calculateButton->Name = L"calculateButton";
	this->calculateButton->Size = System::Drawing::Size(442, 37);
	this->calculateButton->TabIndex = 2;
	this->calculateButton->Text = L"Calculate";
	this->calculateButton->UseVisualStyleBackColor = true;
	this->calculateButton->Click += gcnew System::EventHandler(this, &Form1::calculateButton_Click);
	// 
	// hkoglPanelControl2
	// 
	hkcoglPanelCameraSetting1->Far = 1000;
	hkcoglPanelCameraSetting1->Fov = 45;
	hkcoglPanelCameraSetting1->Near = -1000;
	hkcoglPanelCameraSetting1->Type = HKOGLPanel::HKCOGLPanelCameraSetting::CAMERATYPE::ORTHOGRAPHIC;
	this->hkoglPanelControl2->Camera_Setting = hkcoglPanelCameraSetting1;
	this->hkoglPanelControl2->Dock = System::Windows::Forms::DockStyle::Fill;
	this->hkoglPanelControl2->Location = System::Drawing::Point(3, 45);
	this->hkoglPanelControl2->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
	this->hkoglPanelControl2->Name = L"hkoglPanelControl2";
	hkcoglPanelPixelFormat1->Accumu_Buffer_Bits = HKOGLPanel::HKCOGLPanelPixelFormat::PIXELBITS::BITS_0;
	hkcoglPanelPixelFormat1->Alpha_Buffer_Bits = HKOGLPanel::HKCOGLPanelPixelFormat::PIXELBITS::BITS_0;
	hkcoglPanelPixelFormat1->Stencil_Buffer_Bits = HKOGLPanel::HKCOGLPanelPixelFormat::PIXELBITS::BITS_0;
	this->hkoglPanelControl2->Pixel_Format = hkcoglPanelPixelFormat1;
	this->hkoglPanelControl2->Size = System::Drawing::Size(442, 407);
	this->hkoglPanelControl2->TabIndex = 1;
	this->hkoglPanelControl2->Load += gcnew System::EventHandler(this, &Form1::hkoglPanelControl2_Load);
	this->hkoglPanelControl2->Paint += gcnew System::Windows::Forms::PaintEventHandler(this, &Form1::hkoglPanelControl2_Paint);
	// 
	// openFileDialog1
	// 
	this->openFileDialog1->FileName = L"openFileDialog1";
	// 
	// BottomToolStripPanel
	// 
	this->BottomToolStripPanel->Location = System::Drawing::Point(0, 0);
	this->BottomToolStripPanel->Name = L"BottomToolStripPanel";
	this->BottomToolStripPanel->Orientation = System::Windows::Forms::Orientation::Horizontal;
	this->BottomToolStripPanel->RowMargin = System::Windows::Forms::Padding(3, 0, 0, 0);
	this->BottomToolStripPanel->Size = System::Drawing::Size(0, 0);
	// 
	// TopToolStripPanel
	// 
	this->TopToolStripPanel->Location = System::Drawing::Point(0, 0);
	this->TopToolStripPanel->Name = L"TopToolStripPanel";
	this->TopToolStripPanel->Orientation = System::Windows::Forms::Orientation::Horizontal;
	this->TopToolStripPanel->RowMargin = System::Windows::Forms::Padding(3, 0, 0, 0);
	this->TopToolStripPanel->Size = System::Drawing::Size(0, 0);
	// 
	// RightToolStripPanel
	// 
	this->RightToolStripPanel->Location = System::Drawing::Point(0, 0);
	this->RightToolStripPanel->Name = L"RightToolStripPanel";
	this->RightToolStripPanel->Orientation = System::Windows::Forms::Orientation::Horizontal;
	this->RightToolStripPanel->RowMargin = System::Windows::Forms::Padding(3, 0, 0, 0);
	this->RightToolStripPanel->Size = System::Drawing::Size(0, 0);
	// 
	// LeftToolStripPanel
	// 
	this->LeftToolStripPanel->Location = System::Drawing::Point(0, 0);
	this->LeftToolStripPanel->Name = L"LeftToolStripPanel";
	this->LeftToolStripPanel->Orientation = System::Windows::Forms::Orientation::Horizontal;
	this->LeftToolStripPanel->RowMargin = System::Windows::Forms::Padding(3, 0, 0, 0);
	this->LeftToolStripPanel->Size = System::Drawing::Size(0, 0);
	// 
	// ContentPanel
	// 
	this->ContentPanel->Font = (gcnew System::Drawing::Font(L"Microsoft JhengHei", 9, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
		static_cast<System::Byte>(136)));
	this->ContentPanel->Size = System::Drawing::Size(1264, 681);
	// 
	// hkoglPanelControl1
	// 
	this->hkoglPanelControl1->AutoSize = true;
	this->hkoglPanelControl1->AutoValidate = System::Windows::Forms::AutoValidate::Disable;
	hkcoglPanelCameraSetting2->Far = 1000;
	hkcoglPanelCameraSetting2->Fov = 45;
	hkcoglPanelCameraSetting2->Near = -1000;
	hkcoglPanelCameraSetting2->Type = HKOGLPanel::HKCOGLPanelCameraSetting::CAMERATYPE::ORTHOGRAPHIC;
	this->hkoglPanelControl1->Camera_Setting = hkcoglPanelCameraSetting2;
	this->hkoglPanelControl1->Dock = System::Windows::Forms::DockStyle::Fill;
	this->hkoglPanelControl1->Location = System::Drawing::Point(2, 3);
	this->hkoglPanelControl1->Margin = System::Windows::Forms::Padding(2, 3, 2, 3);
	this->hkoglPanelControl1->Name = L"hkoglPanelControl1";
	hkcoglPanelPixelFormat2->Accumu_Buffer_Bits = HKOGLPanel::HKCOGLPanelPixelFormat::PIXELBITS::BITS_0;
	hkcoglPanelPixelFormat2->Alpha_Buffer_Bits = HKOGLPanel::HKCOGLPanelPixelFormat::PIXELBITS::BITS_0;
	hkcoglPanelPixelFormat2->Stencil_Buffer_Bits = HKOGLPanel::HKCOGLPanelPixelFormat::PIXELBITS::BITS_0;
	this->hkoglPanelControl1->Pixel_Format = hkcoglPanelPixelFormat2;
	this->hkoglPanelControl1->Size = System::Drawing::Size(800, 600);
	this->hkoglPanelControl1->TabIndex = 0;
	this->hkoglPanelControl1->Load += gcnew System::EventHandler(this, &Form1::hkoglPanelControl1_Load);
	this->hkoglPanelControl1->Paint += gcnew System::Windows::Forms::PaintEventHandler(this, &Form1::hkoglPanelControl1_Paint);
	this->hkoglPanelControl1->MouseDown += gcnew System::Windows::Forms::MouseEventHandler(this, &Form1::hkoglPanelControl1_MouseDown);
	this->hkoglPanelControl1->MouseMove += gcnew System::Windows::Forms::MouseEventHandler(this, &Form1::hkoglPanelControl1_MouseMove);
	this->hkoglPanelControl1->MouseWheel += gcnew System::Windows::Forms::MouseEventHandler(this, &Form1::hkoglPanelControl1_MouseWheel);
	// 
	// tableLayoutPanel1
	// 
	this->tableLayoutPanel1->ColumnCount = 2;
	this->tableLayoutPanel1->ColumnStyles->Add((gcnew System::Windows::Forms::ColumnStyle(System::Windows::Forms::SizeType::Percent,
		63.99046F)));
	this->tableLayoutPanel1->ColumnStyles->Add((gcnew System::Windows::Forms::ColumnStyle(System::Windows::Forms::SizeType::Percent,
		36.00954F)));
	this->tableLayoutPanel1->Controls->Add(this->hkoglPanelControl1, 0, 0);
	this->tableLayoutPanel1->Controls->Add(this->tableLayoutPanel2, 1, 0);
	this->tableLayoutPanel1->Dock = System::Windows::Forms::DockStyle::Fill;
	this->tableLayoutPanel1->Location = System::Drawing::Point(3, 28);
	this->tableLayoutPanel1->Name = L"tableLayoutPanel1";
	this->tableLayoutPanel1->RowCount = 1;
	this->tableLayoutPanel1->RowStyles->Add((gcnew System::Windows::Forms::RowStyle(System::Windows::Forms::SizeType::Percent, 100)));
	this->tableLayoutPanel1->Size = System::Drawing::Size(1258, 606);
	this->tableLayoutPanel1->TabIndex = 0;
	// 
	// tableLayoutPanel2
	// 
	this->tableLayoutPanel2->ColumnCount = 1;
	this->tableLayoutPanel2->ColumnStyles->Add((gcnew System::Windows::Forms::ColumnStyle(System::Windows::Forms::SizeType::Percent,
		100)));
	this->tableLayoutPanel2->Controls->Add(this->hkoglPanelControl2, 0, 1);
	this->tableLayoutPanel2->Controls->Add(this->calculateButton, 0, 0);
	this->tableLayoutPanel2->Controls->Add(this->loadTexButton, 0, 4);
	this->tableLayoutPanel2->Controls->Add(this->newSelectButton, 0, 5);
	this->tableLayoutPanel2->Controls->Add(this->splitContainer2, 0, 2);
	this->tableLayoutPanel2->Controls->Add(this->splitContainer3, 0, 3);
	this->tableLayoutPanel2->Dock = System::Windows::Forms::DockStyle::Fill;
	this->tableLayoutPanel2->Location = System::Drawing::Point(807, 3);
	this->tableLayoutPanel2->Name = L"tableLayoutPanel2";
	this->tableLayoutPanel2->RowCount = 6;
	this->tableLayoutPanel2->RowStyles->Add((gcnew System::Windows::Forms::RowStyle()));
	this->tableLayoutPanel2->RowStyles->Add((gcnew System::Windows::Forms::RowStyle(System::Windows::Forms::SizeType::Percent, 100)));
	this->tableLayoutPanel2->RowStyles->Add((gcnew System::Windows::Forms::RowStyle(System::Windows::Forms::SizeType::Absolute, 30)));
	this->tableLayoutPanel2->RowStyles->Add((gcnew System::Windows::Forms::RowStyle(System::Windows::Forms::SizeType::Absolute, 30)));
	this->tableLayoutPanel2->RowStyles->Add((gcnew System::Windows::Forms::RowStyle()));
	this->tableLayoutPanel2->RowStyles->Add((gcnew System::Windows::Forms::RowStyle()));
	this->tableLayoutPanel2->Size = System::Drawing::Size(448, 600);
	this->tableLayoutPanel2->TabIndex = 1;
	// 
	// splitContainer2
	// 
	this->splitContainer2->Dock = System::Windows::Forms::DockStyle::Fill;
	this->splitContainer2->Location = System::Drawing::Point(3, 457);
	this->splitContainer2->Name = L"splitContainer2";
	// 
	// splitContainer2.Panel1
	// 
	this->splitContainer2->Panel1->Controls->Add(this->rotateButton);
	// 
	// splitContainer2.Panel2
	// 
	this->splitContainer2->Panel2->Controls->Add(this->rotateTextBox);
	this->splitContainer2->Size = System::Drawing::Size(442, 24);
	this->splitContainer2->SplitterDistance = 147;
	this->splitContainer2->TabIndex = 7;
	// 
	// splitContainer3
	// 
	this->splitContainer3->Dock = System::Windows::Forms::DockStyle::Fill;
	this->splitContainer3->Location = System::Drawing::Point(3, 487);
	this->splitContainer3->Name = L"splitContainer3";
	// 
	// splitContainer3.Panel1
	// 
	this->splitContainer3->Panel1->Controls->Add(this->scaleButton);
	// 
	// splitContainer3.Panel2
	// 
	this->splitContainer3->Panel2->Controls->Add(this->scaleTextbox);
	this->splitContainer3->Size = System::Drawing::Size(442, 24);
	this->splitContainer3->SplitterDistance = 147;
	this->splitContainer3->TabIndex = 8;
	// 
	// tableLayoutPanel3
	// 
	this->tableLayoutPanel3->ColumnCount = 1;
	this->tableLayoutPanel3->ColumnStyles->Add((gcnew System::Windows::Forms::ColumnStyle(System::Windows::Forms::SizeType::Percent,
		100)));
	this->tableLayoutPanel3->Controls->Add(this->menuStrip1, 0, 0);
	this->tableLayoutPanel3->Controls->Add(this->tableLayoutPanel1, 0, 1);
	this->tableLayoutPanel3->Dock = System::Windows::Forms::DockStyle::Fill;
	this->tableLayoutPanel3->Location = System::Drawing::Point(0, 0);
	this->tableLayoutPanel3->Name = L"tableLayoutPanel3";
	this->tableLayoutPanel3->RowCount = 3;
	this->tableLayoutPanel3->RowStyles->Add((gcnew System::Windows::Forms::RowStyle(System::Windows::Forms::SizeType::Absolute, 25)));
	this->tableLayoutPanel3->RowStyles->Add((gcnew System::Windows::Forms::RowStyle(System::Windows::Forms::SizeType::Absolute, 612)));
	this->tableLayoutPanel3->RowStyles->Add((gcnew System::Windows::Forms::RowStyle()));
	this->tableLayoutPanel3->Size = System::Drawing::Size(1264, 681);
	this->tableLayoutPanel3->TabIndex = 2;
	// 
	// Form1
	// 
	this->AutoScaleDimensions = System::Drawing::SizeF(6, 12);
	this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
	this->ClientSize = System::Drawing::Size(1264, 681);
	this->Controls->Add(this->tableLayoutPanel3);
	this->MainMenuStrip = this->menuStrip1;
	this->Margin = System::Windows::Forms::Padding(2);
	this->Name = L"Form1";
	this->Text = L"Form1";
	this->menuStrip1->ResumeLayout(false);
	this->menuStrip1->PerformLayout();
	this->tableLayoutPanel1->ResumeLayout(false);
	this->tableLayoutPanel1->PerformLayout();
	this->tableLayoutPanel2->ResumeLayout(false);
	this->splitContainer2->Panel1->ResumeLayout(false);
	this->splitContainer2->Panel2->ResumeLayout(false);
	this->splitContainer2->Panel2->PerformLayout();
	(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->splitContainer2))->EndInit();
	this->splitContainer2->ResumeLayout(false);
	this->splitContainer3->Panel1->ResumeLayout(false);
	this->splitContainer3->Panel2->ResumeLayout(false);
	this->splitContainer3->Panel2->PerformLayout();
	(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->splitContainer3))->EndInit();
	this->splitContainer3->ResumeLayout(false);
	this->tableLayoutPanel3->ResumeLayout(false);
	this->tableLayoutPanel3->PerformLayout();
	this->ResumeLayout(false);

}

System::Void CppCLRWinformsProject::Form1::hkoglPanelControl1_Load(System::Object^ sender, System::EventArgs^ e)
{
	GLenum res = glewInit();
	if (res != GLEW_OK) 
	{
		fprintf(stderr, "Error: '%s'\n", glewGetErrorString(res));
	}

	scene = new Scene();
	scene->draw();
}

System::Void CppCLRWinformsProject::Form1::hkoglPanelControl1_Paint(System::Object^ sender, System::Windows::Forms::PaintEventArgs^ e)
{
	if (!scene->isTextureUpdated())
	{
		scene->setTextureToMesh(Utils::donNetStringToSTLString(openFileDialog1->FileName));

		scene->setTextureUpdatedState(true);
	}

	auto currentFrame = clock();

	std::cout << (double)(currentFrame - lastFrame) / CLOCKS_PER_SEC << "s" << std::endl;

	scene->draw();
}

System::Void CppCLRWinformsProject::Form1::hkoglPanelControl1_MouseMove(System::Object^ sender, System::Windows::Forms::MouseEventArgs^ e)
{
	/* Mouse0 handler */
	if (e->Button.ToString() == "Left")
	{
		scene->pick(e->X, e->Y);

		hkoglPanelControl1->Invalidate();
	}
	/* Mouse1 handler*/
	else if (e->Button.ToString() == "Right")
	{
		glm::vec4 position(scene->getCameraRef().getPosition(), 1.0f);
		glm::vec4 pivot(scene->getCameraRef().getLookAt(), 1.0f);


		/* Calculate the amount of rotation */

		float deltaAngleX = 2 * glm::pi<float>() / Config::SCR_W;
		float deltaAngleY =		glm::pi<float>() / Config::SCR_H;

		float xAngle = (lastMouse1X - e->X) * deltaAngleX;
		float yAngle = (lastMouse1Y - e->Y) * deltaAngleY;

		// handle camera direction equal to up vector
		float cosAngle = glm::dot(scene->getCameraRef().getViewDir(), scene->getCameraRef().getUpVector());

		if (cosAngle * Utils::sgn(yAngle) > 0.99f)
		{
			yAngle = 0.0f;
		}


		/* Rotate camera around y-axis */

		glm::mat4 rotationMatX(1.0f);
		rotationMatX = glm::rotate(rotationMatX, xAngle, scene->getCameraRef().getUpVector());
		position = (rotationMatX * (position - pivot)) + pivot;


		/* Rotate camera around x-axis*/

		glm::mat4 rotationMatY(1.0f);
		rotationMatY = glm::rotate(rotationMatY, yAngle, scene->getCameraRef().getRightVector());
		glm::vec3 finalPos = (rotationMatY * (position - pivot)) + pivot;


		/* Update camera */

		scene->getCameraRef().setCameraView(finalPos, scene->getCameraRef().getLookAt(), scene->getCameraRef().getUpVector());


		/* Update mouse position for next rotation */

		lastMouse1X = e->X;
		lastMouse1Y = e->Y;

		hkoglPanelControl1->Invalidate();
	}
	else if (e->Button.ToString() == "Middle")
	{
		glm::vec4 position(scene->getCameraRef().getPosition(), 1.0f);
		glm::vec4 pivot(scene->getCameraRef().getLookAt(), 1.0f);

		float deltaX = -(e->X - lastMouse3X) / Config::SCR_W;
		float deltaY = (e->Y - lastMouse3Y) / Config::SCR_H;

		glm::vec3 cameraUp = -glm::cross(scene->getCameraRef().getViewDir(), scene->getCameraRef().getRightVector());

		glm::mat4 translateMat = glm::translate(glm::mat4(1.0f), deltaX * scene->getCameraRef().getRightVector());
		translateMat = glm::translate(translateMat, deltaY * cameraUp);

		position = translateMat * position;
		pivot = translateMat * pivot;

		scene->getCameraRef().setCameraView(position, pivot, scene->getCameraRef().getUpVector());

		lastMouse3X = e->X;
		lastMouse3Y = e->Y;

		hkoglPanelControl1->Invalidate();
	}

}

System::Void CppCLRWinformsProject::Form1::hkoglPanelControl1_MouseDown(System::Object^ sender, System::Windows::Forms::MouseEventArgs^ e)
{
	/* Mouse0 handler*/
	if (e->Button.ToString() == "Left")
	{
		scene->pick(e->X, e->Y);
	}
	/* Mouse1 handler */
	else if (e->Button.ToString() == "Right")
	{
		lastMouse1X = e->X;
		lastMouse1Y = e->Y;
	}
	/* Mouse3 handler */
	else if (e->Button.ToString() == "Middle")
	{
		lastMouse3X = e->X;
		lastMouse3Y = e->Y;
	}
	
	hkoglPanelControl1->Invalidate();
}

System::Void CppCLRWinformsProject::Form1::hkoglPanelControl1_MouseWheel(System::Object^ sender, System::Windows::Forms::MouseEventArgs^ e)
{
	scene->getCameraRef().increaseCameraDistance(e->Delta);
	hkoglPanelControl1->Invalidate();
}

System::Void CppCLRWinformsProject::Form1::pickToolStripMenuItem_Click(System::Object^ sender, System::EventArgs^ e)
{
	scene->setPickMode(PickMode::ADD_FACE);
}

System::Void CppCLRWinformsProject::Form1::deleteToolStripMenuItem_Click(System::Object^ sender, System::EventArgs^ e)
{
	scene->setPickMode(PickMode::DELETE_FACE);
}

System::Void CppCLRWinformsProject::Form1::vertexToolStripMenuItem_Click(System::Object^ sender, System::EventArgs^ e)
{
	scene->setPickMode(PickMode::POINT);
}

System::Void CppCLRWinformsProject::Form1::hkoglPanelControl2_Load(System::Object^ sender, System::EventArgs^ e)
{
	GLenum res = glewInit();
	if (res != GLEW_OK) 
	{
		fprintf(stderr, "Error: '%s'\n", glewGetErrorString(res));
	}

	painter = new TextureParamPainter();
}

System::Void CppCLRWinformsProject::Form1::hkoglPanelControl2_Paint(System::Object^ sender, System::Windows::Forms::PaintEventArgs^ e)
{
	if (!painter->isUpdatedTexture())
	{
		painter->setTexture(Utils::donNetStringToSTLString(openFileDialog1->FileName));

		painter->setTextureUpdatedState(true);
	}

	painter->setMesh2DData(scene->getMeshRef());
	painter->setLineData(getTextureParamVertices());
	painter->draw();
}

System::Void CppCLRWinformsProject::Form1::calculateButton_Click(System::Object^ sender, System::EventArgs^ e)
{
	ConvexCombMap::calc(scene->getSelectedMeshRef());
	scene->updateSelectedMeshTexcoords();
	hkoglPanelControl1->Invalidate();
	hkoglPanelControl2->Invalidate();
}

System::Void CppCLRWinformsProject::Form1::rotateButton_Click(System::Object^ sender, System::EventArgs^ e)
{
	float angle;
	bool isNumeric = float::TryParse(rotateTextBox->Text, angle);

	if (isNumeric)
	{
		ConvexCombMap::rotate(angle);
		scene->updateSelectedMeshTexcoords();
		hkoglPanelControl1->Invalidate();
		hkoglPanelControl2->Invalidate();
	}
}

System::Void CppCLRWinformsProject::Form1::scaleButton_Click(System::Object^ sender, System::EventArgs^ e)
{
	float ratio;
	bool isNumeric = float::TryParse(scaleTextbox->Text, ratio);

	if (isNumeric)
	{
		ConvexCombMap::scale(ratio);
		scene->updateSelectedMeshTexcoords();
		hkoglPanelControl1->Invalidate();
		hkoglPanelControl2->Invalidate();
	}
}

System::Void CppCLRWinformsProject::Form1::loadTexButton_Click(System::Object^ sender, System::EventArgs^ e)
{
	openFileDialog1->InitialDirectory = Environment::CurrentDirectory;
	openFileDialog1->Filter = "Image Files|*.jpg;*.jpeg;*.png;....";

	std::cout << Utils::donNetStringToSTLString(openFileDialog1->InitialDirectory) << std::endl;

	if (openFileDialog1->ShowDialog() == System::Windows::Forms::DialogResult::OK)
	{
		/* Using state here to delay set texture when seperate context actives */
		scene->setTextureUpdatedState(false);
		painter->setTextureUpdatedState(false);

		ConvexCombMap::calc(scene->getSelectedMeshRef());

		hkoglPanelControl1->Invalidate();
		hkoglPanelControl2->Invalidate();
	}
}

System::Void CppCLRWinformsProject::Form1::newSelectButton_Click(System::Object^ sender, System::EventArgs^ e)
{
	scene->setNewSelectMesh();
	painter->setDrawTexture(false);

	hkoglPanelControl1->Invalidate();
	hkoglPanelControl2->Invalidate();
}

System::Void CppCLRWinformsProject::Form1::saveToolStripMenuItem_Click(System::Object^ sender, System::EventArgs^ e)
{
	openFileDialog1->InitialDirectory = Environment::CurrentDirectory;
	openFileDialog1->Filter = "Json Files|*.json;....";

	if (openFileDialog1->ShowDialog() == System::Windows::Forms::DialogResult::OK)
	{
		scene->saveSelectedMeshData(Utils::donNetStringToSTLString(openFileDialog1->FileName));
	}
}

System::Void CppCLRWinformsProject::Form1::loadToolStripMenuItem_Click(System::Object^ sender, System::EventArgs^ e)
{
	openFileDialog1->InitialDirectory = Environment::CurrentDirectory;
	openFileDialog1->Filter = "Json Files|*.json;....";

	if (openFileDialog1->ShowDialog() == System::Windows::Forms::DialogResult::OK)
	{
		scene->loadSelectedMeshData(Utils::donNetStringToSTLString(openFileDialog1->FileName));
	}
}

System::Void CppCLRWinformsProject::Form1::sobelFilterKeyToolStripMenuItem_Click(System::Object^ sender, System::EventArgs^ e) 
{
	scene->toggleFilter();
	hkoglPanelControl1->Invalidate();
}

System::Void CppCLRWinformsProject::Form1::gridToolStripMenuItem_Click(System::Object^ sender, System::EventArgs^ e)
{
	scene->setGridEnable(!scene->isGridEnabled());

	hkoglPanelControl1->Invalidate();
}
