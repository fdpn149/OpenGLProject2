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
	this->hkoglPanelControl1 = (gcnew HKOGLPanel::HKOGLPanelControl());
	this->menuStrip1 = (gcnew System::Windows::Forms::MenuStrip());
	this->settingToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
	this->faceToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
	this->pickToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
	this->deleteToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
	this->vertexToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
	this->testKeyToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
	this->fileToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
	this->saveToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
	this->loadToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
	this->toolStripContainer2 = (gcnew System::Windows::Forms::ToolStripContainer());
	this->newSelectButton = (gcnew System::Windows::Forms::Button());
	this->loadTexButton = (gcnew System::Windows::Forms::Button());
	this->scaleButton = (gcnew System::Windows::Forms::Button());
	this->rotateButton = (gcnew System::Windows::Forms::Button());
	this->calculateButton = (gcnew System::Windows::Forms::Button());
	this->hkoglPanelControl2 = (gcnew HKOGLPanel::HKOGLPanelControl());
	this->openFileDialog1 = (gcnew System::Windows::Forms::OpenFileDialog());
	this->menuStrip1->SuspendLayout();
	this->toolStripContainer2->ContentPanel->SuspendLayout();
	this->toolStripContainer2->TopToolStripPanel->SuspendLayout();
	this->toolStripContainer2->SuspendLayout();
	this->SuspendLayout();
	// 
	// hkoglPanelControl1
	// 
	this->hkoglPanelControl1->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Bottom)
		| System::Windows::Forms::AnchorStyles::Left)
		| System::Windows::Forms::AnchorStyles::Right));
	this->hkoglPanelControl1->AutoSize = true;
	this->hkoglPanelControl1->AutoValidate = System::Windows::Forms::AutoValidate::Disable;
	hkcoglPanelCameraSetting1->Far = 1000;
	hkcoglPanelCameraSetting1->Fov = 45;
	hkcoglPanelCameraSetting1->Near = -1000;
	hkcoglPanelCameraSetting1->Type = HKOGLPanel::HKCOGLPanelCameraSetting::CAMERATYPE::ORTHOGRAPHIC;
	this->hkoglPanelControl1->Camera_Setting = hkcoglPanelCameraSetting1;
	this->hkoglPanelControl1->Location = System::Drawing::Point(11, 25);
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
	this->hkoglPanelControl1->MouseDown += gcnew System::Windows::Forms::MouseEventHandler(this, &Form1::hkoglPanelControl1_MouseDown);
	this->hkoglPanelControl1->MouseMove += gcnew System::Windows::Forms::MouseEventHandler(this, &Form1::hkoglPanelControl1_MouseMove);
	this->hkoglPanelControl1->MouseWheel += gcnew System::Windows::Forms::MouseEventHandler(this, &Form1::hkoglPanelControl1_MouseWheel);
	// 
	// menuStrip1
	// 
	this->menuStrip1->Dock = System::Windows::Forms::DockStyle::None;
	this->menuStrip1->Items->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(2) {
		this->settingToolStripMenuItem,
			this->fileToolStripMenuItem
	});
	this->menuStrip1->Location = System::Drawing::Point(0, 0);
	this->menuStrip1->Name = L"menuStrip1";
	this->menuStrip1->Size = System::Drawing::Size(1264, 24);
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
	// testKeyToolStripMenuItem
	// 
	this->testKeyToolStripMenuItem->Name = L"testKeyToolStripMenuItem";
	this->testKeyToolStripMenuItem->Size = System::Drawing::Size(132, 22);
	this->testKeyToolStripMenuItem->Text = L"TestKey";
	this->testKeyToolStripMenuItem->Click += gcnew System::EventHandler(this, &Form1::testKeyToolStripMenuItem_Click);
	// 
	// fileToolStripMenuItem
	// 
	this->fileToolStripMenuItem->DropDownItems->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(2) {
		this->saveToolStripMenuItem,
			this->loadToolStripMenuItem
	});
	this->fileToolStripMenuItem->Name = L"fileToolStripMenuItem";
	this->fileToolStripMenuItem->Size = System::Drawing::Size(38, 20);
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
	// toolStripContainer2
	// 
	this->toolStripContainer2->BottomToolStripPanelVisible = false;
	// 
	// toolStripContainer2.ContentPanel
	// 
	this->toolStripContainer2->ContentPanel->Controls->Add(this->newSelectButton);
	this->toolStripContainer2->ContentPanel->Controls->Add(this->loadTexButton);
	this->toolStripContainer2->ContentPanel->Controls->Add(this->scaleButton);
	this->toolStripContainer2->ContentPanel->Controls->Add(this->rotateButton);
	this->toolStripContainer2->ContentPanel->Controls->Add(this->calculateButton);
	this->toolStripContainer2->ContentPanel->Controls->Add(this->hkoglPanelControl2);
	this->toolStripContainer2->ContentPanel->Controls->Add(this->hkoglPanelControl1);
	this->toolStripContainer2->ContentPanel->Size = System::Drawing::Size(1264, 657);
	this->toolStripContainer2->Dock = System::Windows::Forms::DockStyle::Fill;
	this->toolStripContainer2->LeftToolStripPanelVisible = false;
	this->toolStripContainer2->Location = System::Drawing::Point(0, 0);
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
	// newSelectButton
	// 
	this->newSelectButton->Font = (gcnew System::Drawing::Font(L"微軟正黑體", 12));
	this->newSelectButton->Location = System::Drawing::Point(855, 476);
	this->newSelectButton->Name = L"newSelectButton";
	this->newSelectButton->Size = System::Drawing::Size(248, 37);
	this->newSelectButton->TabIndex = 6;
	this->newSelectButton->Text = L"Select new mesh";
	this->newSelectButton->UseVisualStyleBackColor = true;
	this->newSelectButton->Click += gcnew System::EventHandler(this, &Form1::newSelectButton_Click);
	// 
	// loadTexButton
	// 
	this->loadTexButton->Font = (gcnew System::Drawing::Font(L"微軟正黑體", 12));
	this->loadTexButton->Location = System::Drawing::Point(855, 433);
	this->loadTexButton->Name = L"loadTexButton";
	this->loadTexButton->Size = System::Drawing::Size(248, 37);
	this->loadTexButton->TabIndex = 5;
	this->loadTexButton->Text = L"Load Texture";
	this->loadTexButton->UseVisualStyleBackColor = true;
	this->loadTexButton->Click += gcnew System::EventHandler(this, &Form1::loadTexButton_Click);
	// 
	// scaleButton
	// 
	this->scaleButton->Font = (gcnew System::Drawing::Font(L"微軟正黑體", 12));
	this->scaleButton->Location = System::Drawing::Point(855, 390);
	this->scaleButton->Name = L"scaleButton";
	this->scaleButton->Size = System::Drawing::Size(248, 37);
	this->scaleButton->TabIndex = 4;
	this->scaleButton->Text = L"Scale 1.25x";
	this->scaleButton->UseVisualStyleBackColor = true;
	this->scaleButton->Click += gcnew System::EventHandler(this, &Form1::scaleButton_Click);
	// 
	// rotateButton
	// 
	this->rotateButton->Font = (gcnew System::Drawing::Font(L"微軟正黑體", 12, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
		static_cast<System::Byte>(136)));
	this->rotateButton->Location = System::Drawing::Point(855, 347);
	this->rotateButton->Name = L"rotateButton";
	this->rotateButton->Size = System::Drawing::Size(248, 37);
	this->rotateButton->TabIndex = 3;
	this->rotateButton->Text = L"Rotate 30 degree";
	this->rotateButton->UseVisualStyleBackColor = true;
	this->rotateButton->Click += gcnew System::EventHandler(this, &Form1::rotateButton_Click);
	// 
	// calculateButton
	// 
	this->calculateButton->Font = (gcnew System::Drawing::Font(L"微軟正黑體", 12, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
		static_cast<System::Byte>(136)));
	this->calculateButton->Location = System::Drawing::Point(855, 25);
	this->calculateButton->Name = L"calculateButton";
	this->calculateButton->Size = System::Drawing::Size(248, 37);
	this->calculateButton->TabIndex = 2;
	this->calculateButton->Text = L"Calculate";
	this->calculateButton->UseVisualStyleBackColor = true;
	this->calculateButton->Click += gcnew System::EventHandler(this, &Form1::calculateButton_Click);
	// 
	// hkoglPanelControl2
	// 
	hkcoglPanelCameraSetting2->Far = 1000;
	hkcoglPanelCameraSetting2->Fov = 45;
	hkcoglPanelCameraSetting2->Near = -1000;
	hkcoglPanelCameraSetting2->Type = HKOGLPanel::HKCOGLPanelCameraSetting::CAMERATYPE::ORTHOGRAPHIC;
	this->hkoglPanelControl2->Camera_Setting = hkcoglPanelCameraSetting2;
	this->hkoglPanelControl2->Location = System::Drawing::Point(855, 68);
	this->hkoglPanelControl2->Name = L"hkoglPanelControl2";
	hkcoglPanelPixelFormat2->Accumu_Buffer_Bits = HKOGLPanel::HKCOGLPanelPixelFormat::PIXELBITS::BITS_0;
	hkcoglPanelPixelFormat2->Alpha_Buffer_Bits = HKOGLPanel::HKCOGLPanelPixelFormat::PIXELBITS::BITS_0;
	hkcoglPanelPixelFormat2->Stencil_Buffer_Bits = HKOGLPanel::HKCOGLPanelPixelFormat::PIXELBITS::BITS_0;
	this->hkoglPanelControl2->Pixel_Format = hkcoglPanelPixelFormat2;
	this->hkoglPanelControl2->Size = System::Drawing::Size(248, 259);
	this->hkoglPanelControl2->TabIndex = 1;
	this->hkoglPanelControl2->Load += gcnew System::EventHandler(this, &Form1::hkoglPanelControl2_Load);
	this->hkoglPanelControl2->Paint += gcnew System::Windows::Forms::PaintEventHandler(this, &Form1::hkoglPanelControl2_Paint);
	// 
	// openFileDialog1
	// 
	this->openFileDialog1->FileName = L"openFileDialog1";
	// 
	// Form1
	// 
	this->AutoScaleDimensions = System::Drawing::SizeF(6, 12);
	this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
	this->ClientSize = System::Drawing::Size(1264, 681);
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

		glm::mat4 translateMat = glm::translate(glm::mat4(1.0f), deltaX * scene->getCameraRef().getRightVector());
		translateMat = glm::translate(translateMat, deltaY * scene->getCameraRef().getUpVector());

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

System::Void CppCLRWinformsProject::Form1::testKeyToolStripMenuItem_Click(System::Object^ sender, System::EventArgs^ e)
{

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
	ConvexCombMap::rotate(5.0f);
	scene->updateSelectedMeshTexcoords();
	hkoglPanelControl1->Invalidate();
	hkoglPanelControl2->Invalidate();
}

System::Void CppCLRWinformsProject::Form1::scaleButton_Click(System::Object^ sender, System::EventArgs^ e)
{
	ConvexCombMap::scale(0.8f);
	scene->updateSelectedMeshTexcoords();
	hkoglPanelControl1->Invalidate();
	hkoglPanelControl2->Invalidate();
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
	painter->setTexture("");

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