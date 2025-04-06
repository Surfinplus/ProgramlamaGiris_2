/*****************************************************************************************************************************************************************************************************
**                                                      SAKARYA ÜNÝVERSÝTESÝ
**                                            BÝLGÝSAYAR VE BÝLÝÞÝM BÝLÝMLERÝ FAKÜLTESÝ
**                                                BÝLGÝSAYAR MÜHENDÝSLÝÐÝ BÖLÜMÜ
**                                                   PROGRAMLAMAYA GÝRÝÞ DERSÝ
**                                                     2022-2023 GÜZ DÖNEMÝ 
**
**
**  ÖDEV NUMARASI: PROJE
**  ÖÐRENCÝ ADI : EREN ÇOBAN
**  ÖÐRENCÝ NUMARASI:G221210090
**  DERS GRUBU: A
**
**
**
**
**
******************************************************************************************************************************************************************************************/



#include <fstream>
#include <iostream>
#include <string>
#include <algorithm>
#include <cctype>

using namespace std;

class InterfaceAyrikIslem//interface
{
public:
    //asagidaki islemler yemek ve urun icinde bulunmasi gereken fakat farkli sekilde kodlanmasi gereken islemler.
    //bu yuzden basliklari inteface icinde tanimlanir govdeleri ise classlar icinde yazilir
    virtual void gunlukUretim(string ad) = 0;
    virtual void VeriSil(string silinecek) = 0;
    virtual void VeriGuncelle(string guncellenecek) = 0;
    virtual void Raporla() = 0;
};

class OrtakIslem {//kalitim veren class
protected:
    void IslemKaydet(string dosyaAdi, string kaydedilecek) {//kaydetme islemi yemek ve urun icin farklilik gostermediginden bu classta tanimlanip kalitimla her iki classa aktarilir
        ofstream WriteUrun;
        WriteUrun.open(dosyaAdi, ios::app);
        WriteUrun << kaydedilecek << endl;//veri kaydedilir
        WriteUrun.close();
    }
};

class KarMaliyyet : public OrtakIslem {
public:
    float kisibasi;
    float toplam;

    KarMaliyyet() {
        kisibasi = 0;
        toplam = 0;
    }

    void ToplamMaliyetHesapla() {
        ifstream ReadDosya("gunluk_uretim.txt");
        string tmp1, tmp2, tmp3;
        float veri1, veri2, veri3;
        while (true) {
            ReadDosya >> tmp1 >> veri1 >> tmp2 >> veri2 >> veri3 >> tmp3;//dosyadan veriler okunur
            if (ReadDosya.eof()) break;
            kisibasi = kisibasi + veri2;
            toplam = toplam + veri3;
        }

        cout << "Uretilen yemeklerin toplam kisi basi maliyet tutari - " << kisibasi << endl;
        cout << "Uretilen yemeklerin toplam maliyet tutari - " << toplam << endl;
    }

    void Listele() {
        ifstream ReadDosya("gunluk_uretim.txt");
        while (true) {
            string tmp1, tmp2, tmp3;
            float veri1, veri2, veri3;
            ReadDosya >> tmp1 >> veri1 >> tmp2 >> veri2 >> veri3 >> tmp3;//dosyadan veriler okunur
            if (ReadDosya.eof()) break;
            cout << "Yemek adi - " << tmp1 << endl << "Uretilen yemek miktari - " << veri1 << endl;
        }
        ReadDosya.close();
    }

    void YemekSat(string secim) {
        bool kontrol = false;
        ofstream WriteGecici("gecici.tmp");
        ifstream ReadDosya("gunluk_uretim.txt");
        string tmp1, tmp2;
        float veri1, veri2, veri3, veri4, adet = 0;
        while (true) {
            ReadDosya >> tmp1 >> veri1 >> veri2 >> veri3 >> veri4 >> tmp2;//dosyadan veriler okunur
            if (ReadDosya.eof()) break;
            if (secim == tmp1) {
                cout << "Kac adet satilacak? seciminiz : "; cin >> adet;
                if (veri1 >= adet) {
                    kontrol = true;
                    cout << "Odenen fiyat : " << veri1 * veri2 << " tl" << endl;
                    IslemKaydet("satislar.txt", to_string(veri1 * veri2));
                    WriteGecici << tmp1 << endl << veri1 - adet << endl << veri2 << endl << veri3 << endl << veri4 << endl << tmp2 << endl;
                }
                else {
                    kontrol = true;
                    cout << "Girdiginiz adette yemek uretilmemistir." << endl;
                    WriteGecici << tmp1 << endl << veri1 << endl << veri2 << endl << veri3 << endl << veri4 << endl << tmp2 << endl;
                }
            }
            else {
                WriteGecici << tmp1 << endl << veri1 << endl << veri2 << endl << veri3 << endl << veri4 << endl << tmp2 << endl;
            }
        }
        if (!kontrol)
            cout << "Girdiginiz yemek bugun uretilmemistir" << endl;
        ReadDosya.close();
        WriteGecici.close();
        remove("gunluk_uretim.txt");//eski dosya silinir
        rename("gecici.tmp", "gunluk_uretim.txt");//gecici dosyanin ismi degistirilir
    }

    void KarMaliyetHesapla() {
        ifstream ReadDosya("satislar.txt");
        ifstream ReadUretim("gunluk_uretim.txt");
        string tmp1, tmp2;
        float veri1, veri2, veri3, veri4, kar = 0;
        while (true) {
            ReadUretim >> tmp1 >> veri1 >> veri2 >> veri3 >> veri4 >> tmp2;//dosyadan veriler okunur
            if (ReadUretim.eof()) break;
            toplam = toplam + veri4;
        }
        while (true)
        {
            ReadDosya >> veri1;
            if (ReadDosya.eof()) break;
            kar = kar + veri1;
        }

        cout << "Satislardan elde edilen toplam para - " << kar << endl;
        cout << "Uretilen yemeklerin toplam maliyet tutari - " << toplam << endl;
        cout << "Aradaki fark = " << kar - toplam << endl;
        ReadDosya.close();
    }
};

class Urun : public OrtakIslem, public InterfaceAyrikIslem {//OrtakIslem sinifindan kalitim alir ayni zamanda InterfaceAyrikIslem interfaceini de implement eder
private:
    string urunAd;//urun adi
    string uretimTarihi;//uretim tarihi
    string sonKullanma;//son kullanma tarihi
    float kalori;//kalori bilgisi
    string stokTuru;//urunun stokta kilogramla mi yoksa adetlemi tutuldugu bilgisi
    float stok;//urun stok bilgisi
    float fiyat;//urun fiyat bilgisi
    string final_data;//verilerin birlestirildigi string degisken
    string dosyaAd;//kayit yapilacak dosyanin adi
    string blank;//dosyadaki kayit sonunu gosteren karakteri okumada kullanilacak degisken
public:
    Urun() {
        dosyaAd = "depo.txt";
    }
    void gunlukUretim(string ad) {

    }
    void VeriKaydet() {
        cout << "(bosluk yerine _ kullanin)\n";
        cout << "Urun adi - "; cin >> urunAd;
        cout << "Uretim tarihi - "; cin >> uretimTarihi;
        cout << "Son kullanim tarihi - "; cin >> sonKullanma;
        cout << "Kalori miktari - "; cin >> kalori;
        cout << "Stokta bulundurulma turu(kg,lt,adet) - "; cin >> stokTuru;
        cout << "Stok miktari - "; cin >> stok;
        cout << "Fiyat(tl) - "; cin >> fiyat;//kullanici verileri girer
        final_data = urunAd + "\n" + uretimTarihi + "\n" + sonKullanma + "\n" + to_string(kalori) + "\n" + stokTuru + "\n" + to_string(stok) + "\n" + to_string(fiyat) + "\n" + "~";//kullanicidan alinan degerler sirasiyla uygun sekilde string degiskende toplanir
        transform(final_data.begin(), final_data.end(), final_data.begin(), [](unsigned char c) { return std::tolower(c); });//tum kayitlar kucuk karakterle yapilir
        IslemKaydet(dosyaAd, final_data);//kayit islemi icin fonksiyon cagrilir ve urun kaydedilir
    }
    void VeriSil(string silinecek) {
        bool bulunamadi = true;
        ifstream ReadUrun(dosyaAd);
        ofstream WriteGecici("gecici.tmp");
        while (true) {
            ReadUrun >> urunAd >> uretimTarihi >> sonKullanma >> kalori >> stokTuru >> stok
                >> fiyat >> blank;//dosyadan veriler okunur
            if (ReadUrun.eof()) break;
            if (silinecek == urunAd)
            {
                //silinecek olan urun adina sahip kayit gecici dosyaya aktarilmaz boylece silinmis olur
                bulunamadi = false;
                cout << "\nSilme islemi basarili\n";//islem gerceklestirildikten sonra ekrana mesaj yazdirilir
            }
            else
                WriteGecici << urunAd << endl << uretimTarihi << endl << sonKullanma
                << endl << kalori << endl << stokTuru << endl << stok
                << endl << fiyat << endl << "~" << endl;//diger veriler oldugu gibi gecici dosyaya aktarilir
        }
        if (bulunamadi)
            cout << "\nUrun adi kayitli degil\n";//kayit bulunamazsa ekrana mesaj yazdirilir
        WriteGecici.close();
        ReadUrun.close();
        remove("depo.txt");//eski dosya silinir
        rename("gecici.tmp", "depo.txt");//gecici dosyanin ismi degistirilir
    }
    void VeriGuncelle(string guncellenecek) {
        bool bulunamadi = true;
        ifstream ReadUrun(dosyaAd);
        ofstream WriteGecici("gecici.tmp");
        while (true) {
            ReadUrun >> urunAd >> uretimTarihi >> sonKullanma >> kalori >> stokTuru >> stok
                >> fiyat >> blank;//veriler dosyadan okunur
            if (ReadUrun.eof()) break;
            if (guncellenecek == urunAd)//aranan kayit bulunursa burasi calisir
            {
                cout << "Urun adi - " << urunAd;
                cout << "\nUretim tarihi - " << uretimTarihi;
                cout << "\nSon kullanim tarihi - " << sonKullanma;
                cout << "\nKalori miktari - " << kalori;
                cout << "\nStokta bulundurulma turu(kg,lt,adet) - " << stokTuru;
                cout << "\nStok miktari - " << stok;
                cout << "\nFiyat(tl) - " << fiyat;//eski veriler ekrana yazilir
                cout << "\n------------------------------------------\n";
                cout << "(bosluk yerine _ kullanin)\n";
                cout << "Uretim tarihi - "; cin >> uretimTarihi;
                cout << "Son kullanim tarihi - "; cin >> sonKullanma;
                cout << "Kalori miktari - "; cin >> kalori;
                cout << "Stokta bulundurulma turu(kg,lt,adet) - "; cin >> stokTuru;
                cout << "Stok miktari - "; cin >> stok;
                cout << "Fiyat(tl) - "; cin >> fiyat;//yeni veriler kullanicidan alinir
                transform(urunAd.begin(), urunAd.end(), urunAd.begin(), [](unsigned char c) { return std::tolower(c); });//kayit islemlerinde tum kayitlar kucuk harfle yapilir
                WriteGecici << urunAd << endl << uretimTarihi << endl << sonKullanma
                    << endl << kalori << endl << stokTuru << endl << stok
                    << endl << fiyat << endl << "~" << endl;//guncellenmek istenen kayit icin yeni degiskenler kullanicidan alindiktan sonra gecici dosyaya kaydedilir
                bulunamadi = false;//kaydin bulundugunu gosteren degiskenin degeri degistirilir
                cout << "\nGuncelleme islemi basarili\n";//islemin gerceklestirildigi mesaji ekrana yazilir
            }
            else
                WriteGecici << urunAd << endl << uretimTarihi << endl << sonKullanma
                << endl << kalori << endl << stokTuru << endl << stok
                << endl << fiyat << endl << "~" << endl;//geri kalan kayitlar degistirilmeden gecici dosyaya aktarilir
        }
        if (bulunamadi)
            cout << "\nUrun adi kayitli degil\n";//kayit bulunamazsa ekrana mesaj yazarak belirtilir
        WriteGecici.close();
        ReadUrun.close();
        remove("depo.txt");//eski dosya silinir
        rename("gecici.tmp", "depo.txt");//gecici dosyanin ismi degistirilir
    }
    void Raporla() {//bu metot dosyada bulunan kayitlari duzenli bicimde kullaniciya gosterir
        ifstream ReadUrun(dosyaAd);
        int sayac = 1;//kayit sayisini tutan degisken
        while (true) {
            ReadUrun >> urunAd >> uretimTarihi >> sonKullanma >> kalori >> stokTuru >> stok//veriler dosyadan sirasiyla okunur
                >> fiyat >> blank;
            if (ReadUrun.eof()) break;//dosya sonuna ulasildiginda dongu biter
            cout << "---" + to_string(sayac) + " numarali urun-----------------------" << endl;
            cout << "Urun adi - " << urunAd;
            cout << "\nUretim tarihi - " << uretimTarihi;
            cout << "\nSon kullanim tarihi - " << sonKullanma;
            cout << "\nKalori miktari - " << kalori;
            cout << "\nStokta bulundurulma turu(kg,lt,adet) - " << stokTuru;
            cout << "\nStok miktari - " << stok;
            cout << "\nFiyat(tl) - " << fiyat << endl;
            sayac++;
        }
        cout << "------------------------------------------\n";
        ReadUrun.close();
    }
};

class Yemek : public OrtakIslem, public InterfaceAyrikIslem {
private:
    string yemekAd;//yemek degiskenleri
    string yemekAdMalzeme;
    float yemekFiyat;
    int malzemeSayisi;//yemek icin kac malzeme gerekli

    string malzemeAd;//malzeme degiskenleri
    string miktarTuru;
    float malzemeMiktar;
    string malzemeler;

    string dosyaAd1;//yemekcesit.txt
    string dosyaAd2;//malzeme.txt
    string final_data1;
    string final_data2;
    string blank;
public:
    Yemek() {//kurucu fonksiyon
        dosyaAd1 = "yemekcesit.txt";
        dosyaAd2 = "malzeme.txt";
    }
    void gunlukUretim(string ad) {
        int adedi = 0;
        bool kontrol = true;
        cout << ad << " yemeginden gunluk kac adet uretilecegini girin : "; cin >> adedi;
        float hesapYemek = 0;
        float hesap = 0;
        float toplam_maliyet = 0;
        Urun* urun = new Urun();
        bool bulunamadi = true;
        ifstream ReadYemek(dosyaAd1);
        while (true) {
            ReadYemek >> yemekAd >> yemekFiyat >> malzemeSayisi >> blank;//yemek verileri okunur
            if (ReadYemek.eof()) break;
            if (ad == yemekAd)//guncellenecek kayit bulunur
            {
                ifstream ReadMalzeme(dosyaAd2);
                while (true) {
                    ReadMalzeme >> yemekAdMalzeme;//malzeme verileri dosyadan okunmaya baslar
                    if (ReadMalzeme.eof()) break;
                    if (ad == yemekAdMalzeme) {//yemege ait olan malzemeler bulunur
                        int sayac = 0;//kullaniciya uygun sirayla malzeme bilgisi mesajlarini yazdirmak icin kullanilan degisken
                        final_data1 = final_data1 + ad + "\n" + to_string(adedi) + "\n" + to_string(yemekFiyat);
                        while (malzemeler != blank)
                        {
                            if (sayac != 0) {
                                float gereken = 1;
                                if (sayac == 3) {
                                    bool bulunamadi2 = true;
                                    string tmp1, tmp2, tmp3, tmp4, tmp5, tmp6, tmp7, tmp8;
                                    ifstream ReadUrun("depo.txt");
                                    ofstream WriteGecici("gecici.tmp");
                                    gereken = adedi * stof(malzemeler);
                                    while (true) {
                                        ReadUrun >> tmp1 >> tmp2 >> tmp3 >> tmp4 >> tmp5 >> tmp6
                                            >> tmp7 >> tmp8;//dosyadan veriler okunur
                                        if (ReadUrun.eof()) break;
                                        if (malzemeAd == tmp1)
                                        {
                                            float sonuc = stof(tmp6) - gereken;
                                            if (sonuc >= 0) {
                                                hesap = hesap + stof(tmp7) * stof(malzemeler);
                                                WriteGecici << tmp1 << endl << tmp2 << endl << tmp3
                                                    << endl << tmp4 << endl << tmp5 << endl << sonuc
                                                    << endl << tmp7 << endl << tmp8 << endl;
                                                bulunamadi2 = false;
                                            }
                                            else {
                                                gereken = gereken - stof(tmp6);
                                                WriteGecici << tmp1 << endl << tmp2 << endl << tmp3
                                                    << endl << tmp4 << endl << tmp5 << endl << tmp6
                                                    << endl << tmp7 << endl << tmp8 << endl;
                                            }
                                        }
                                        else
                                            WriteGecici << tmp1 << endl << tmp2 << endl << tmp3
                                            << endl << tmp4 << endl << tmp5 << endl << tmp6
                                            << endl << tmp7 << endl << tmp8 << endl;//diger veriler oldugu gibi gecici dosyaya aktarilir
                                    }
                                    if (bulunamadi2) {
                                        string siparisler = malzemeAd + " " + to_string(gereken);
                                        hesap = hesap + 0;
                                        IslemKaydet("siparis.txt", siparisler);
                                        if (kontrol) {
                                            cout << "\nDepoda gerekli urun adi kayitli degil. Siparis listesine urun adi ve miktari eklendi.\n";//kayit bulunamazsa ekrana mesaj yazdirilir
                                            kontrol = false;
                                        }
                                    }

                                    WriteGecici.close();
                                    ReadUrun.close();
                                    remove("depo.txt");//eski dosya silinir
                                    rename("gecici.tmp", "depo.txt");//gecici dosyanin ismi degistirilir
                                    toplam_maliyet = adedi * hesap;
                                    //
                                    sayac = 0;
                                }
                                else if (sayac == 2) {

                                }
                                else {
                                    malzemeAd = malzemeler;
                                }
                            }
                            ReadMalzeme >> malzemeler;
                            sayac++;
                        }
                        final_data1 = final_data1 + "\n" + to_string(hesap) + "\n" + to_string(toplam_maliyet);
                    }
                    malzemeler = "";//degisken dongu adimi sonunda sifirlanir
                }
                ReadMalzeme.close();
                bulunamadi = false;
            }
        }
        ReadYemek.close();
        final_data1 = final_data1 + "\n~";
        transform(final_data1.begin(), final_data1.end(), final_data1.begin(), [](unsigned char c) { return std::tolower(c); });

        if (bulunamadi)//kayit bulunamazsa ekrana mesaj yazdirilir
            cout << "\nYemek adi kayitli degil\n";
        else {
            if (kontrol) {
                IslemKaydet("gunluk_uretim.txt", final_data1);
            }
        }
        final_data1 = "";
    }
    void VeriKaydet() {
        cout << "(bosluk yerine _ kullanin)\n";
        cout << "Yemek adi - "; cin >> yemekAd;//kullanicidan veriler alinir
        cout << "Yemek fiyati(tl) - "; cin >> yemekFiyat;
        cout << "Malzeme sayi - "; cin >> malzemeSayisi;//Yemek icin gerekli malzemelerin sayisi
        final_data1 = yemekAd + "\n" + to_string(yemekFiyat) + "\n" + to_string(malzemeSayisi) + "\n" + "~";//yemek bilgileri ayri bir string icinde uygun sirayla toplanir
        final_data2 = yemekAd;
        for (int i = 0; i < malzemeSayisi; i++) {//yemek icin gerekli malzemeler kullanicidan tek tek alinir
            cout << "Malzeme adi - "; cin >> malzemeAd;
            cout << "Malzeme miktarinin turu(kg,lt,adet) - "; cin >> miktarTuru;
            cout << "Malzeme miktari - "; cin >> malzemeMiktar;
            final_data2 = final_data2 + "\n" + malzemeAd + "\n" + miktarTuru + "\n" + to_string(malzemeMiktar);//her seferinde veriler malzeme bilgisini tutan degiskende toplanir
        }
        final_data2 = final_data2 + "\n~";//degiskenin sonuna kayit sonunu gosterecek karakterimiz eklenir
        transform(final_data1.begin(), final_data1.end(), final_data1.begin(), [](unsigned char c) { return std::tolower(c); });//veriler kucuk karakterlere donusturlur
        transform(final_data2.begin(), final_data2.end(), final_data2.begin(), [](unsigned char c) { return std::tolower(c); });
        IslemKaydet(dosyaAd1, final_data1);//veriler dosyaya kaydedilir
        IslemKaydet(dosyaAd2, final_data2);
    }
    void VeriSil(string silinecek) {
        bool bulunamadi = true;
        ifstream ReadYemek(dosyaAd1);
        ofstream WriteGecici1("gecici1.tmp");
        while (true) {
            ReadYemek >> yemekAd >> yemekFiyat >> malzemeSayisi >> blank;//oncelikle yemekcesit.txtden yemek bilgisi okunur
            if (ReadYemek.eof()) break;
            if (silinecek == yemekAd)//eger yemek adi kayitliysa calisir
            {
                ifstream ReadMalzeme(dosyaAd2);
                ofstream WriteGecici2("gecici2.tmp");
                while (true) {
                    ReadMalzeme >> yemekAdMalzeme;//malzeme txtden veriler okunmaya baslar
                    if (ReadMalzeme.eof()) break;
                    if (silinecek == yemekAdMalzeme) {//silinecek yemege ait malzemeler bulundugunda calisir
                        while (malzemeler != blank)
                        {
                            ReadMalzeme >> malzemeler;//kayit sonun karakterine kadar malzeme verileri okunur ama gecici dosyaya aktarilmaz boylelikle silinmis olur
                        }
                    }
                    else {//diger tum kayit verileri gecici dosyaya aktarilir
                        WriteGecici2 << yemekAdMalzeme << endl;
                        while (malzemeler != blank)
                        {
                            ReadMalzeme >> malzemeler;
                            WriteGecici2 << malzemeler << endl;
                        }
                    }
                    malzemeler = "";//her dongu adimi sonunda degisken sifirlanir
                }
                ReadMalzeme.close();
                WriteGecici2.close();
                remove("malzeme.txt");//eski dosya silinir
                rename("gecici2.tmp", "malzeme.txt");//gecici dosya eski dosyanin yerini alir
                bulunamadi = false;
                cout << "\nSilme islemi basarili\n";//kullaniciya islemin basarili oldugu bilgisi yazdirilir
            }
            else
                WriteGecici1 << yemekAd << endl << yemekFiyat << endl << malzemeSayisi
                << endl << "~" << endl;//diger yemek bilgileri oldugu gubu gecici dosyaya aktarilir
        }
        if (bulunamadi)//yemek adi kayitli degilse calisir ve kullaniciya mesaj gosterilir
            cout << "\nYemek adi kayitli degil\n";
        WriteGecici1.close();
        ReadYemek.close();
        remove("yemekcesit.txt");//eski dosya silinir
        rename("gecici1.tmp", "yemekcesit.txt");//gecici dosya eski dosyanin yerine gecer
    }
    void VeriGuncelle(string guncellenecek) {
        bool bulunamadi = true;
        ifstream ReadYemek(dosyaAd1);
        ofstream WriteGecici1("gecici1.tmp");
        while (true) {
            ReadYemek >> yemekAd >> yemekFiyat >> malzemeSayisi >> blank;//yemek verileri okunur
            if (ReadYemek.eof()) break;
            if (guncellenecek == yemekAd)//guncellenecek kayit bulunur
            {
                cout << "Yemek adi - " << yemekAd << endl;
                cout << "Yeni yemek adi - "; cin >> yemekAd;
                cout << "Yemek fiyati(tl) - " << yemekFiyat << endl;
                cout << "Yeni yemek fiyati - "; cin >> yemekFiyat;//yeni degerler kullanicidan alinir
                ifstream ReadMalzeme(dosyaAd2);
                ofstream WriteGecici2("gecici2.tmp");
                while (true) {
                    ReadMalzeme >> yemekAdMalzeme;//malzeme verileri dosyadan okunmaya baslar
                    if (ReadMalzeme.eof()) break;
                    if (guncellenecek == yemekAdMalzeme) {//yemege ait olan malzemeler bulunur
                        int sayac = 0;//kullaniciya uygun sirayla malzeme bilgisi mesajlarini yazdirmak icin kullanilan degisken
                        WriteGecici2 << yemekAd << endl;
                        while (malzemeler != blank)
                        {
                            if (sayac != 0) {
                                if (sayac == 3) {
                                    cout << "Malzeme miktari - " << malzemeler << endl;
                                    cout << "Yeni malzeme miktari - "; cin >> malzemeler;
                                    WriteGecici2 << malzemeler << endl;//kullanicidan yeni veriler alinir ve dosyaya kaydedilir
                                    sayac = 0;
                                }
                                else if (sayac == 2) {
                                    cout << "Stokta bulundurulma turu(kg,lt,adet) - " << malzemeler << endl;
                                    cout << "YeniStokta bulundurulma turu(kg,lt,adet) - "; cin >> malzemeler;
                                    WriteGecici2 << malzemeler << endl;//kullanicidan yeni veriler alinir ve dosyaya kaydedilir
                                }
                                else {
                                    cout << "Malzeme adi - " << malzemeler << endl;
                                    cout << "Yeni malzeme adi - "; cin >> malzemeler;
                                    WriteGecici2 << malzemeler << endl;//kullanicidan yeni veriler alinir ve dosyaya kaydedilir
                                }
                            }
                            ReadMalzeme >> malzemeler;
                            if (malzemeler == blank)
                                WriteGecici2 << malzemeler << endl;//son olarak kayit sonunu belirten karakter de dosyaya eklenir
                            sayac++;
                        }

                    }
                    else {
                        WriteGecici2 << yemekAdMalzeme << endl;
                        while (malzemeler != blank)
                        {
                            ReadMalzeme >> malzemeler;
                            WriteGecici2 << malzemeler << endl;//diger kayitlar degistirilmeden kaydedilir
                        }
                    }
                    malzemeler = "";//degisken dongu adimi sonunda sifirlanir
                }
                WriteGecici1 << yemekAd << endl << yemekFiyat << endl << malzemeSayisi << endl << "~" << endl;//kullanicinin girdigi veriler gecici dosyaya kaydedilir
                ReadMalzeme.close();
                WriteGecici2.close();
                remove("malzeme.txt");//eski dosya silinir
                rename("gecici2.tmp", "malzeme.txt");//gecici dosya eski dosyanin yerine gecer
                bulunamadi = false;
                cout << "\nGuncelleme islemi basarili\n";//kullaniciya mesaj gosterilir
            }
            else
                WriteGecici1 << yemekAd << endl << yemekFiyat << endl << malzemeSayisi//diger veriler oldugu gibi aktarilir
                << endl << "~" << endl;
        }
        if (bulunamadi)//kayit bulunamazsa ekrana mesaj yazdirilir
            cout << "\nYemek adi kayitli degil\n";
        WriteGecici1.close();
        ReadYemek.close();
        remove("yemekcesit.txt");//eski dosya silinir
        rename("gecici1.tmp", "yemekcesit.txt");//yeni dosya
    }
    void Raporla() {
        ifstream ReadYemek(dosyaAd1);
        int sayac = 1;
        while (true) {
            ReadYemek >> yemekAd >> yemekFiyat >> malzemeSayisi >> blank;//veriler okunur
            if (ReadYemek.eof()) break;
            cout << "---" + to_string(sayac) + " numarali yemek-----------------------" << endl;
            cout << "Yemek adi - " << yemekAd;
            cout << "\nYemek fiyati(tl) - " << yemekFiyat;
            cout << "\nMalzeme sayisi - " << malzemeSayisi << endl;//yemek bilgileri ekrana yazdirilir
            ifstream ReadMalzeme(dosyaAd2);
            while (true) {
                ReadMalzeme >> yemekAdMalzeme;
                if (ReadMalzeme.eof()) break;
                int sayac1 = 0;
                while (malzemeler != blank)//kayit sonuna karakterine kadar devam eder
                {
                    if (yemekAdMalzeme == yemekAd) {//gecerli yemek icin malzeme kayitlari bulunur
                        if (sayac1 != 0) {
                            if (sayac1 == 3) {
                                cout << "Malzeme miktari - " << malzemeler << endl;//malzeme bilgileri ekrana yazdirilir
                                sayac1 = 0;
                            }
                            else if (sayac1 == 2) {
                                cout << "Stokta bulundurulma turu(kg,lt,adet) - " << malzemeler << endl;//malzeme bilgileri ekrana yazdirilir
                            }
                            else {
                                cout << "Malzeme adi - " << malzemeler << endl;//malzeme bilgileri ekrana yazdirilir
                            }
                        }
                        sayac1++;
                    }
                    ReadMalzeme >> malzemeler;
                }
                malzemeler = "";
            }
            ReadMalzeme.close();
            sayac++;
        }
        cout << "------------------------------------------\n";
        ReadYemek.close();
    }
};

class KullaniciEtkilesim {//kullanicin etkilesime girdigi elementlerin ornegin menu bulundugu sinif
private:
    string menu = "Urun(depo.txt) islemleri\n[1]-Urun ekle\n[2]-Urun sil\n[3]-Urun guncelle\n[4]-Urun raporla\n\nYemek cesiti(yemekcesit.txt,malzeme.txt) islemleri\n[5]-Yemek ekle\n[6]-Yemek sil\n[7]-Yemek guncelle\n[8]-Yemek raporla\n\nDiger islemler\n[9]-Gunluk yemek uretimi belirle\n[10]-Genel,kisi basi maliyet tutari hesapla\n[11]-Yemek sat\n[12]-Kar maliyet raporu\n\nSeciminiz : ";
public:
    string secim = "";//islem secimi degiskeni

    void Yazdir() {
        while (secim != "E" && secim != "e") {//kullanici e veya E girmedigi surece program sonlanmaz
            cout << menu; cin >> secim;//menu yazdirilir ve islem sectirilir
            if (secim == "1") {//urun kayit
                system("cls");
                Urun* urun = new Urun();
                urun->VeriKaydet();//uygun sinifin islemi yerine getiren metodu cagrilir
                delete urun;//sinifin nesnesi yok edilir
            }
            else if (secim == "2") {//urun sil
                system("cls");
                Urun* urun = new Urun();
                cout << "Silmek istediginiz urunun adini giriniz(bosluk yerine _ kullanin) : "; cin >> secim;
                urun->VeriSil(secim);//uygun sinifin islemi yerine getiren metodu cagrilir
                delete urun;//sinifin nesnesi yok edilir
            }
            else if (secim == "3") {//urun guncelle
                system("cls");
                Urun* urun = new Urun();
                cout << "Guncellemek istediginiz urunun adini giriniz(bosluk yerine _ kullanin) : "; cin >> secim;
                urun->VeriGuncelle(secim);//uygun sinifin islemi yerine getiren metodu cagrilir
                delete urun;//sinifin nesnesi yok edilir
            }
            else if (secim == "4") {//urun raporla
                system("cls");
                Urun* urun = new Urun();
                urun->Raporla();//uygun sinifin islemi yerine getiren metodu cagrilir
                delete urun;//sinifin nesnesi yok edilir
            }
            else if (secim == "5") {//yemek kayit
                system("cls");
                Yemek* yemek = new Yemek();
                yemek->VeriKaydet();//uygun sinifin islemi yerine getiren metodu cagrilir
                delete yemek;//sinifin nesnesi yok edilir
            }
            else if (secim == "6") {//yemek sil
                system("cls");
                Yemek* yemek = new Yemek();
                cout << "Silmek istediginiz yemegin adini giriniz(bosluk yerine _ kullanin) : "; cin >> secim;
                yemek->VeriSil(secim);//uygun sinifin islemi yerine getiren metodu cagrilir
                delete yemek;//sinifin nesnesi yok edilir
            }
            else if (secim == "7") {//yemek guncelle
                system("cls");
                Yemek* yemek = new Yemek();
                cout << "Guncellemek istediginiz yemegin adini giriniz(bosluk yerine _ kullanin) : "; cin >> secim;
                yemek->VeriGuncelle(secim);//uygun sinifin islemi yerine getiren metodu cagrilir
                delete yemek;//sinifin nesnesi yok edilir
            }
            else if (secim == "8") {//yemek raporla
                system("cls");
                Yemek* yemek = new Yemek();
                yemek->Raporla();//uygun sinifin islemi yerine getiren metodu cagrilir
                delete yemek;//sinifin nesnesi yok edilir
            }
            else if (secim == "9") {//gunluk yemek uretimi belirle
                system("cls");
                //dosya sil
                string yemekAd;
                cout << "Kac adet yemek uretilecegini girin. secimin : "; cin >> secim;
                Yemek* yemek = new Yemek();
                yemek->Raporla();
                cout << "Yemek adlari yukaridaki listeden secilmelidir. Yanlis veya hatali girilen yemek adlari icin uretim yapilmaz.\n------------------------------------------------------------------\n";
                for (int i = 0; i < stoi(secim); i++) {
                    cout << "Uretilecek " << i + 1 << "'inci yemegin adini girin : "; cin >> yemekAd;
                    yemek->gunlukUretim(yemekAd);
                }
                delete yemek;
            }
            else if (secim == "10") {//genel, kisi basi maliyet tutari
                system("cls");
                KarMaliyyet* nesne1 = new KarMaliyyet();
                nesne1->ToplamMaliyetHesapla();
                delete nesne1;
            }
            else if (secim == "11") {//yemek sat
                system("cls");
                KarMaliyyet* nesne1 = new KarMaliyyet();
                nesne1->Listele();
                cout << "Uretilen yemeklerden satmak istediginizin adini giriniz : "; cin >> secim;
                nesne1->YemekSat(secim);
            }
            else if (secim == "12") {//kar maliyet raporu
                system("cls");
                KarMaliyyet* nesne1 = new KarMaliyyet();
                nesne1->KarMaliyetHesapla();
            }
            cout << "Program sonlandirilsin mi?(E/H) - "; cin >> secim;//her islem sonunda kullanicya devam etmek isteyip istemedigi sorulur
            system("cls");
        }
    }
}islemListesi;//bu siniftan sadece bir kez nesne uretilecegi icin nesneyi sinif tanimlamasi sonunda urettim

int main()
{
    islemListesi.Yazdir();
    cin.get();
    return 0;
}