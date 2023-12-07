# NewYearCardHtml

## 概要
住所録のファイルと、あらかじめ用意した裏面の画像を使ってHTMLで年賀状の原稿を作成できます。  
Linux環境のみを想定しています。  
Windowsだとshift-jisとutf8の問題がちょっと...。

## 使い方
一番上が差出人の情報になります。
コマンドで
```
NewYearCardHtml 住所録.csv
```
という風にする、もしくは住所録を実行ファイルにドラッグアンドドロップするとHTMLが出力されます。

### CSVの各データの項目名
* 氏名
    + name
    + 名前
    + 氏名

* 住所
    + address
    + 住所
    + 自宅住所
 
* 郵便番号
    + postal code
    + 郵便番号
    + 自宅郵便番号
 
* 裏面
    + picture
    + image
    + 画像
    + 文面
    + 裏面
