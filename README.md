🚗 Araç Kiralama Yönetim Sistemi (C Programlama)
Bu proje, temel C programlama tekniklerini kullanarak bir araç kiralama şirketinin operasyonlarını yönetmek için tasarlanmış bir terminal uygulamasıdır. Sistem; araç, müşteri ve kiralama bilgilerini metin dosyalarında saklayarak veri kalıcılığı sağlar.

🛠 Özellikler
Sistem üç ana modülden oluşmaktadır:

1. Araç Yönetimi
Ekleme: Yeni araçları marka, model ve yıl bilgisiyle sisteme kaydeder.

Silme: ID bazlı araç silme işlemi yapar.

Arama & Listeleme: Mevcut araçların durumunu (Müsait/Kirada) görüntüler.

2. Müşteri Yönetimi
TC Kimlik Doğrulama: 11 haneli ve sayısal kontrolü yapılmış TC kimlik numaraları ile kayıt.

Arama: Müşteri bilgilerine hızlı erişim.

3. Kiralama İşlemleri
Kiralama: Sadece müsait araçlar ve kayıtlı müşteriler arasında bağ kurar.

Teslim Alma: Kiralanan aracın durumunu tekrar "Müsait" olarak günceller.

Geçmiş Listeleme: Kimin hangi aracı kiraladığını raporlar.

📁 Dosya Yapısı
Program verileri aşağıdaki .txt dosyalarında saklar:

araclar.txt: Araç envanter verileri.

musteriler.txt: Kayıtlı müşteri bilgileri.

kiralamalar.txt: Kiralama kayıtları.

🚀 Kurulum ve Çalıştırma
Sistemde bir C derleyicisinin (GCC önerilir) kurulu olması yeterlidir.

Derleme:

Bash

gcc main.c -o arac_kiralama

Çalıştırma:

Bash

./arac_kiralama

💻 Kullanılan Teknikler
Dinamik Bellek Yönetimi: malloc ve realloc kullanılarak araç listesi çalışma zamanında genişletilir.

Dosya Giriş/Çıkış (I/O): fscanf ve fprintf ile kalıcı veri depolama.

Veri Yapıları: İlişkili verileri tutmak için struct kullanımı.


Hata Kontrolü: Geçersiz girişleri (sayı yerine harf girilmesi vb.) engellemek için getSafeInt ve clearBuffer fonksiyonları.

