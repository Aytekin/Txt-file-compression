/**
* @file Hufman.cpp
* @description Huffman algoritmasi kullanarak bir metin dosyas�n�n boyutunu daraltmak
* @course 2.��retin B grubu
* @assignment 04
* @date 27.12.2017
* @author Aytekin Erlale - Ahmet �zdilli / aytekin.erlale@ogr.sakarya.edu.tr - ahmet.ozdilli@ogr.sakarya.edu.tr 
*/
#include "Huffman.hpp"

Huffman::Huffman(string dosyaYolu)throw(string){
			harfler = 256;
			girdi="";
			try{
				DosyaOku(dosyaYolu);
			}
			catch(string ex){
				throw ex;
			}
			root=NULL;
		}
void Huffman::DosyaOku(string dosyaYolu)throw(string){
	string satir;
	ifstream dosya(dosyaYolu.c_str());
	if (dosya.is_open())
	{
		while (getline(dosya,satir))
		{
			girdi+=satir;
			girdi+='\n';
		}
		dosya.close();
	}
	else throw "Dosya acilamadi"; 
}
void Huffman::Kodla(){
	char karakterler[girdi.size()-1];
	
	strcpy(karakterler, girdi.c_str());
	
	//Frekanslar� say
	int frek[harfler]; 
	// frekanslar� s�f�rla
	for(int i=0;i<harfler;i++)frek[i] = 0;
	
	for (int i = 0; i < girdi.size()-1; i++)
		frek[karakterler[i]]++;
	
	// Huffman a�ac�n� olu�tur
	root = AgacOlustur(frek);
	
	// 01 kodlar olu�turuluyor
	KodOlustur(root, "");
}	
Dugum* Huffman::AgacOlustur(int frek[]){
	// dosyada var olan karakterler i�in d���m olu�tur.
	Dugum* dugumler[harfler];
	for(int i=0;i<harfler;i++){
		if(frek[i]>0){
			dugumler[i] = new Dugum((char)i,frek[i]);
		}
		else{
			dugumler[i] = NULL;
		}
	}
	while(true){
		int ilkIndeks = MinDugumIndeks(dugumler);				
		Dugum *min1 = dugumler[ilkIndeks];
		dugumler[ilkIndeks] = NULL;
		int ikinciIndeks = MinDugumIndeks(dugumler);
		if(ikinciIndeks == -1){
			return min1;
		}
		Dugum *min2 = dugumler[ikinciIndeks];
		dugumler[ikinciIndeks] = NULL;
		dugumler[ilkIndeks] = new Dugum(0,min1->frekans+min2->frekans,min1,min2);
	}
	
}
int Huffman::MinDugumIndeks(Dugum* dugumler[]){
	Dugum* min;
	int minIndeks=-1;
	
	// ilk null olmayan d���m� min al
	for(int i=0;i<harfler;i++){
		if(dugumler[i] != NULL){
			min = dugumler[i];
			minIndeks = i;
			break;
		}
	}
	
	for(int i=0;i<harfler;i++)
	{
		if(dugumler[i] == NULL) continue;
		
		if(*min > *(dugumler[i])) {
			min = dugumler[i];
			minIndeks = i;
		}
	}
	return minIndeks;
}
void Huffman::KodOlustur(Dugum* alt_dugum,string kod){
	if(!alt_dugum->Yaprakmi()){
		KodOlustur(alt_dugum->sol,kod+'0');
		KodOlustur(alt_dugum->sag,kod+'1');
	}
	else{
		alt_dugum->kod = kod;
	}
}
void Huffman::TabloYaz(char alt_dugum,string kod){
	//gelen 1010 yollari tablo txt ye kaydedilme methodu
	ofstream Yaz;
	Yaz.open("tablo.txt",ios::app);
	
	//mesela 1001 geldi bu sayinin karsiligi ASCII'de 
	
	if(alt_dugum == ' ')
		Yaz<<'@'<<' '<<kod<<"\n";
	else if(alt_dugum == '\n')
		Yaz<<'>'<<' '<<kod<<"\n";
	else
		Yaz<<alt_dugum<<' '<<kod<<"\n";	
}
ostream& operator<<(ostream& ekran,Huffman& sag){
	ekran<<"Huffman Agacindaki Karakterler ve Kodlari:"<<endl;
	sag.inorder(sag.root);
	return ekran;
}
void Huffman::inorder(Dugum* alt_dugum){
	if(alt_dugum != NULL){
		inorder(alt_dugum->sol);
		if(alt_dugum->Yaprakmi())
		{
			TabloYaz(alt_dugum->karakter,alt_dugum->kod); 

		}

		inorder(alt_dugum->sag);
	}
}
void Huffman::SilDugum(Dugum* alt_dugum){
	if(alt_dugum == NULL) return;
	SilDugum(alt_dugum->sol);
	SilDugum(alt_dugum->sag);
	delete alt_dugum;
	alt_dugum = NULL;
}
Huffman::~Huffman(){
	// ��p olu�mamas� i�in a�a� temizleniyor.
	SilDugum(root);	
}