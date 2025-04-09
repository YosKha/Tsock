from sys import getsizeof

_MASK32 = 2 ** 32 - 1
_MASK64 = 2 ** 64 - 1
_CONST = 6364136223846793005


def state_setseq_64(state=0, inc=0):
    """
    ilk çalıştırmada state ve inc değerlerini 0 olarak ata,
    ve bu değişkenleri 2**64-1 ile AND işlemine koy
    ilk değerler 0 olduğu için dizi [0,0] olarak tanımlanacaktır.
    """
    return [state & _MASK64, inc & _MASK64]


random_t = state_setseq_64


def srandom_r(rng, initstate, initseq):
    rng[::] = [0, (initseq << 1) & _MASK64 | 1]
    """
        rgn değişkeni yeni değerleri için initseq 1 bit sola kaydır.
        gelen değer 2**64-1 ile AND işlemine koy
        daha sonra sonuç 1 ile OR işlemine koy
    """
    random_r(rng)
    rng[0] += initstate  # rng[0] yani Xo değerine initstate ekle
    rng[0] &= _MASK64  # Xo değerini 2**64-1 değeri ile AND işlemi uygula Xo'ı güncelle
    random_r(rng)  # yeni Xo ve c değerlerini (rng) tekrardan random_r methoduna gönder


def random_r(rng):
    oldstate, inc = rng
    """
    oldstate, inc değerleri güncelle
    """
    rng[0] = (oldstate * _CONST + inc) & _MASK64
    """
        formül X(n+1) = ((Xn * a) + c) ^ (2**(64-1)) 
        ((0 * 6364136223846793005) + 109) ^ 2**64-1
    """

    xorshifted = (((oldstate >> 18) ^ oldstate) >> 27) & _MASK32
    """
        XORSHIFT

        oldstate değerini 18 bit sağa kaydır 
        gelen sonucu oldstate değeri ile XOR işlemine koy
        sonucu 27 bit sağa kaydır 2**(32-1)) ile AND işlemi uygula
    """
    rot = (oldstate >> 59) & _MASK32

    """
        ROTATE

        oldstate değerini 59 bit sağa kaydır ve 2**(32-1)) ile AND işlemi uygula
    """

    """
        RETURN 

        (1) XORSHIFT'i ROTATE değeri kadar sağa kaydır 
        (2) -ROTATE DEĞERİNİ 31 ile AND işlemi uygula sonucu XORSHIFT değeri kadar sola kaydır.
        (3) 1 ve 2 sonuclarına OR işlemi uygula 
         son olarak (3) sonucuna 2**(32-1)) ile AND işlemi uygula

    """
    return ((xorshifted >> rot) | (xorshifted << ((-rot) & 31))) & _MASK32


if __name__ == '__main__':
    # random sayı üreteci değişkenini array olarak tanımla
    rng = random_t()
    """42 ve 54 ile sabit değerli tohumlar oluştur

        bilgi: 
        lineer congruential generator(LGC) formülü
        X(n+1) = (a**Xn + c) * mod M 
        bize ilk tohum değeri oluşturmak için 
        Xo, M, a, c değerleri gerekmekte
        M için ==> 2**(64-1) veya 2**(32-1)
        a için ==> 6364136223846793005  --> sabit olarak tanımlanıyor.
        c için ==> 54
        X0 için ==> 42 
        tabiki tohum değerleri PCG için farklı işlemlerden geçecektir.
        incelemeye başlayalım.

    """
    srandom_r(rng, 20170707105851, 42)

    '''#sayı çıktıları
    for i in range(6):
            print(random_r(rng))
     '''
    # 32bit sayı çıktıları

    print('')

    for i in range(100000):
        rgnt = random_r(rng)
        s = "{0:b}".format(rgnt)
        zero_count = 0
        if len(s) != 32:
            zero_count = 32 - len(s)

        for y in range(zero_count):
            s = '0' + s

        # rgnt = '%08x' % random_r(rng)

        # print(zero_count)
        # print(s)
        # print()

        rgnt = random_r(rng)
        with open("test.txt", "a") as f:
            f.write(str(s) + '\n')
