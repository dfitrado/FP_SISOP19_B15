# Laporan FP Sisop 2019
### Oleh:
- Donny Fitrado (05111740000171)
- Wildan G. Budhi (05111740000184)

## Soal: Daemon Thread
1. Buatlah program C yang menyerupai crontab menggunakan daemon dan thread. Ada sebuah file crontab.data untuk menyimpan config dari crontab. Setiap ada perubahan file tersebut maka secara otomatis program menjalankan config yang sesuai dengan perubahan tersebut tanpa perlu diberhentikan. Config hanya sebatas * dan 0-9 (tidak perlu /,- dan yang lainnya)

## Solusi:
- Membuat sebuah program Deamon yang dapat membaca file config dan mengecek apabila terjadi perubahan, dan dapat membuat thread sebanyak config yang ada.
- Membuat thread yang digunakan untuk mengecek dan menjalankan setiap config
-  Dilakukan pembuatan proses untuk menjalankan sebuah program

~~ Menyusul ~~