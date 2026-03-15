Armored Vehicle Simulation (Unreal Engine 5 & C++)

Bu proje, bir teknik görev kapsamında Unreal Engine 5 ve C++ kullanılarak geliştirilmiş, çok oyunculu (multiplayer) bir tank simülasyonudur.

🚀 Öne Çıkan Özellikler
1. C++ Tabanlı Araç ve Silah Sistemi

    Tamamen C++ Kontrolü: Araç hareketi, taret dönüşü ve namlu açılandırması Unreal'ın Enhanced Input sistemiyle C++ üzerinden optimize edilmiştir.

    Server-Authoritative Atış Sistemi: Atış mekanikleri tamamen sunucu yetkisindedir. İstemci (Client) ateş etme isteği gönderir, sunucu mermiyi yaratır ve tüm oyunculara senkronize eder.

2. Gelişmiş Multiplayer Mimari

    Rol Tabanlı Oynanış: Dinamik bir UI (Widget) üzerinden oyuncular Sürücü veya Nişancı rollerini seçebilir.

    Possess Sistemi: Seçilen role göre oyuncu, araç üzerindeki ilgili Pawn'a C++ üzerinden Possess edilir.

    Ana Menü (Host/Join): Kullanıcı dostu bir arayüz ile yerel ağ (LAN) üzerinden sunucu kurma ve mevcut sunucuya katılma özelliği entegre edilmiştir.

3. Fizik ve Bonus Mekanikler

    Dinamik Süspansiyon: Araç tekerlekleri zeminle fiziksel etkileşime girer, engebeli arazilerde gerçekçi bir tepki verir.

    Gelişmiş Kamera: Fare tekerleği ile 1. şahıs (kokpit) ve 3. şahıs (takip) kameraları arasında yumuşak geçiş.

    Replication: Tüm hareketler ve animasyonlar (MovementComponent ve Variable Replication ile) gecikmesiz şekilde tüm istemcilerde senkronizedir.

🛠️ Teknik Detaylar
Özellik	Kullanılan Teknoloji / Sınıf
Giriş Sistemi	Enhanced Input Component (C++)
Ağ Yapısı	RPC (Client-to-Server) & Variable Replication
Arayüz	UMG & C++ Controller Integration
Fizik	Chaos Vehicle Physics Engine
Fizik 


📦 Kurulum ve Çalıştırma

    Projeyi bilgisayarınıza indirin veya klonlayın: git clone [REPO_LINK]
    
    Simulation.uproject dosyasına sağ tıklayıp "Generate Visual Studio project files" seçeneğini seçin.
    
    .sln dosyasını açın ve projeyi Development Editor modunda derleyin.
    
    Unreal Editor açıldığında Maps/Map_MainMenu haritasını başlatın.
    
    Multiplayer Testi İçin: Play ayarlarından "Number of Players" değerini 2 yapın ve "Net Mode" olarak "Play As Listen Server" seçin.


🎮 Kontroller (Input Mapping)

Proje, Unreal Engine'in yeni Enhanced Input System mimarisini kullanmaktadır. Varsayılan kontrol şeması aşağıdadır:

🚜 Araç Kontrolleri (Sürücü)
Tuş	İşlem

W / S ---->	Gaz / Fren & Geri Vites

A / D ---->	Direksiyon (Sağ / Sol)

Space ---->	El Freni

C ----> FirstPerson/ThirdPerson Kamera geçişi

Kule Kontrolleri (Nişancı)

Tuş	İşlem

W / S ---->  Namlu Elevasyonu

A / D ----> Kule Dönüşü

Mouse R Click ---->	Ana Topu Ateşle (Fire)
