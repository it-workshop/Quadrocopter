Управление квадрокоптером
=========================

Управление квадрокоптером осуществляется по протоколу XBee или через 2.4Ghz RC-канал.


Как установить диспетчерское приложение?
========================================

MacOS X
-------
В данный момент приложение требует Qt4.
Рекомендуется установить Qt SDK с официального сайта qt-project.org, не через Mac Ports.
Одну из последних версий можно скачать по ссылке:

* http://download.qt-project.org/official_releases/qt/4.8/
* http://download.qt-project.org/official_releases/qtcreator/3.0/3.0.1/qt-creator-opensource-mac-x86_64-3.0.1.dmg

После установки Qt клонируем репозиторий нашего проекта:

    $ mkdir ~/workspace/qcopter
    
    $ cd qcopter
    
    $ git clone https://github.com/it-workshop/Quadrocopter.git
    
    $ cd control/qt


Далее потребуется скачать и установить Qwt5 и QExtSerialPort.

Начнём с простого:

    $ mkdir 3rdparty
    
    $ cd 3rdparty
    
    $ git clone https://code.google.com/p/qextserialport/
    

В файл ../quadro.pro внизу добавляем:

    include ( 3rdparty/qextserialport.pri )

Готово! Теперь нужно установить Qwt:


    $ cd /tmp/build/
    
    $ svn checkout svn://svn.code.sf.net/p/qwt/code/branches/qwt-5.2
    
    $ cd qwt-5.2

    $ qmake -spec macx-g++
    
    $ make
    
    $ make install
    
    $ sudo ln -s /usr/local/qwt-5.2.0/lib/qwt.framework/qwt /usr/lib/qwt
    
    
Остаётся лишь подключить библиотеку в quadro.pro:

    mac: QMAKEFEATURES += /usr/local/qwt-6.0.1/features
    
    CONFIG += qwt
    
    mac: INCLUDEPATH += /usr/local/qwt-6.0.1/lib/qwt.framework/Headers
    
    mac: LIBS += -F/usr/local/qwt-6.0.1/lib -framework qwt


Почитать
--------
* Тут пока ничего нет.
