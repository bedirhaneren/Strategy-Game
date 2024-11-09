# 🐉 **ORKMANIA**
Bu oyun senaryo tabanlı bir 2D oyundur. Proje dosya açma kapama, veri okuma, index yapıları ve pointerlar konularını pekiştirmek için geliştirilmiştir. Dosyaların çekildiği verileri proje dizinindeki *Files* klasörü içerisinde bulabilirsiniz.

# **Oyunun Açıklaması** :
Bu projede, İnsan İmparatorluğu ve Ork Lejyonu arasındaki bir strateji tabanlı ızgara oyunu simüle edilmektedir. Her iki tarafın kendilerine ait temel birimleri bulunmaktadır. Bu birimlerin saldırı, savunma, sağlık ve kritik vuruş oranları gibi çeşitli özellikleri vardır.

Ayrıca, her iki tarafın kahramanları ve canavarları da bulunmaktadır. Kahramanlar ve canavarlar, takımlarına çeşitli avantajlar sağlar ve bu avantajlar, temel birimlerin güçlerini artırabilir. Bu kahramanlar, oyunda bulunan kritik vuruş mekanizmasını da etkileyebilirler. Kritik vuruş, belirli turlarda şansa bağlı olarak gerçekleşir ve normal saldırılardan %50 daha fazla hasar verir. Her bir birimin kendine ait bir kritik vuruş şansı mevcuttur. Belirli kahramanlar, belirli birimlerin kritik vuruş şansını artırabilir. Eğer takımınızda o kahraman varsa, kritik vuruş şansınız artacak ve daha avantajlı bir hale geleceksiniz.

Her iki takımın da araştırma etkileri bulunmaktadır. Toplamda 4 adet araştırma etkisi vardır bunlar saldırı geliştirmesi, savunma geliştirmesi, kuşatma ustalığı , elit eğitim gibi etkilerdir. Bu araştırma etkileri, belirli birimlere saldırı gücü, savunma gücü veya kritik vuruş şansı gibi temel özelliklerin değerlerini artırabilir. Bu araştırma etkileri senaryo verilerinden çekilir. Daha sonra sağladığı avantajlar ilgili birimlere uygulanır.

Oyunda bir yorgunluk mekanizması da bulunmaktadır. Birimler her 5 turda bir yorulurlar ve saldırı güçleri %10 azalır. Tüm bu etkiler, oyunu daha stratejik bir hale getirmekte ve çeşitlilik sağlamaktadır.

# **Yöntem**
Oyunda yer alan birimlerin her birinin görselleri, PNG formatındaki dosyalardan SDL_image kütüphanesi kullanılarak yüklenmektedir. Bu görseller, senaryoya bağlı olarak SDL2 ile oluşturduğumuz 20x20 kare şeklindeki savaş alanında konumlanmaktadır. Konumlanmalar bir karede maksimum 100 birlik olacak şekilde ayarlanmıştır. Eğer 100 birimden fazla biriminiz var ise birlikleriniz yeni bir karede doğarlar. Senaryonun ilerleyişine göre, iki takımdan da ölen birimler ekrandan kaldırılır ve kalan birimler, azalmış can miktarlarıyla oyun alanında gösterilmeye devam eder. Bu süreç sonunda savaşın sonucu belirlenir. Savaş sonucunda hangi tarafın oyunu kazandığı ekranda gösterilir.

Tüm bu süreçte, birimlere, kahramanlara, canavarlara ve araştırma etkilerine ait veriler, proje dosyasında bulunan JSON dosyalarından manuel olarak okunur. JSON dosyalarının veri sırası önemli değildir; kod, bu dosyaları veri sırasına bağımsız olarak okuyacak şekilde tasarlanmıştır. Senaryoya ait veriler okulumun vermiş bir linkten hazır kütüphane (CURL) kullanılarak JSON dosyalarından çekilir. Senaryolar sabittir ve kullanıcı tarafından arayüzde değiştirilemez. Fakat manuel olarak bir senaryo yazıp oyundaki sonucunu görebilirsiniz. Okunan veriler, gerekli yapıların değişkenlerine atanır ve bu değişkenlerdeki verilere göre savaş simüle edilir.

# Örnek Görüntüler
<a href="https://hizliresim.com/y9yi089"><img src="https://i.hizliresim.com/y9yi089.jpg" alt="Resim Yükle"></a>
<a href="https://hizliresim.com/ehx7dil"><img src="https://i.hizliresim.com/ehx7dil.jpg" alt="Resim Yükle"></a>
