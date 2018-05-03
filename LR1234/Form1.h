#pragma once
#include"TIntegrator.h"
#include"Ecranoplan.h"
#include <iostream>
#include<fstream>
#include<stdio.h>
#include<Windows.h>
#include<stdlib.h>
using namespace std;
namespace CppCLR_WinformsProjekt {

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
	public:
		Form1(void)
		{
			InitializeComponent();
			//
			//TODO: Konstruktorcode hier hinzufügen.
			//
		}

	protected:
		/// <summary>
		/// Verwendete Ressourcen bereinigen.
		/// </summary>
		~Form1()
		{
			if (components)
			{
				delete components;
			}
		}
	private: System::Windows::Forms::Button^  button1;
	protected:

	private:
		/// <summary>
		/// Erforderliche Designervariable.
		/// </summary>
		System::ComponentModel::Container ^components;

#pragma region Windows Form Designer generated code
		/// <summary>
		/// Erforderliche Methode für die Designerunterstützung.
		/// Der Inhalt der Methode darf nicht mit dem Code-Editor geändert werden.
		/// </summary>
		void InitializeComponent(void)
		{
			this->button1 = (gcnew System::Windows::Forms::Button());
			this->SuspendLayout();
			// 
			// button1
			// 
			this->button1->Location = System::Drawing::Point(12, 21);
			this->button1->Name = L"button1";
			this->button1->Size = System::Drawing::Size(75, 23);
			this->button1->TabIndex = 0;
			this->button1->Text = L"button1";
			this->button1->UseVisualStyleBackColor = true;
			this->button1->Click += gcnew System::EventHandler(this, &Form1::button1_Click);
			// 
			// Form1
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(613, 374);
			this->Controls->Add(this->button1);
			this->Name = L"Form1";
			this->Text = L"Ýêðàíîïëàí";
			this->ResumeLayout(false);

		}
#pragma endregion
	private: System::Void button1_Click(System::Object^  sender, System::EventArgs^  e) {
		string outFileName = "out.txt";
		ofstream fout(outFileName, ios_base::out | ios_base::trunc);
		if (!fout) { cout << "Îøèáêà îòêðûòèÿ ôàéëà" << endl;  system("pause"); }
		fout.setf(ios::fixed);

		TDormanPrinceIntegrator integr(1e-12);
		double T0 = 0.0;
		//double T1 = 11 * 60 * 60 + 15 * 60;
		double T1 = 20;

		Ecranoplan model(T0, T1);
		integr.Run(model);
		for (int i = 0; i < model.Result.size(); i++)
		{
			TVector str = model.Result.front();
			model.Result.pop_front();
			for (int j = 0; j < str.getSize(); j++)
			{
				fout << str[j] << " ";
			}
			fout << endl;
		}
		fout.close();
	}
	};
}
