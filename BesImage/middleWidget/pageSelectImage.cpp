﻿#include "pageSelectImage.h"

PageSelectImage::PageSelectImage(QWidget *parent) : baseWidget(parent),
  fileTree(this),
  imageList(this),
  m_btnhidelist(this),
  sizeIconLast(100,100)
{
    setStyleSheet("PageSelectImage{background:transparent;}");
    setMouseTracking(true);

    initWidget();

    connect(&fileTree,SIGNAL(clicked(const QModelIndex&)),this,SLOT(showSelectedImg(const QModelIndex&)));

    connect(&m_btnhidelist,SIGNAL(clicked(bool)),this,SLOT(slot_btnclicked()));
}


bool PageSelectImage::eventFilter(QObject *o, QEvent *e)
{
    return QObject::eventFilter(o,e);
}

void PageSelectImage::wheelEvent(QWheelEvent *event)
{
    QPoint numPixels = event->pixelDelta();
    QPoint numDegrees = event->angleDelta() / 8;

    if ( QApplication::keyboardModifiers () == Qt::ControlModifier)
    {
        if (!numPixels.isNull()) {
            ScaleImageListOnce(numPixels.y() > 0);
        } else if (!numDegrees.isNull()) {
            QPoint numSteps = numDegrees / 15;
            ScaleImageListOnce(numSteps.y() > 0);
        }

        event->accept();
        return;
    }

    event->ignore();
}

void PageSelectImage::initWidget()
{
    /*初始化fileTree */
    //创建目录 model
    QDirModel *model = new QDirModel(this);
    model->setFilter(QDir::AllDirs|QDir::NoDotAndDotDot|QDir::Files);

    QStringList nameFilters;
    nameFilters << "*.bmp" << "*.png";
    model->setNameFilters(nameFilters);

    model->iconProvider()->setOptions(QFileIconProvider::DontUseCustomDirectoryIcons);

    fileTree.setModel(model);
    QString rootPath = "";//"C:/Users/Benso/"; //="";
    if (!rootPath.isEmpty()) {
        const QModelIndex rootIndex = model->index(QDir::cleanPath(rootPath));
        if (rootIndex.isValid())
            fileTree.setRootIndex(rootIndex);
    }

    // Demonstrating look and feel features
    fileTree.setAnimated(true);
    fileTree.setIndentation(15);
    fileTree.setSortingEnabled(true);
    fileTree.setColumnWidth(0, 300);
    fileTree.setColumnHidden(1, true);
    fileTree.setColumnHidden(2, true);
    fileTree.setColumnHidden(3, true);
    fileTree.setWindowTitle(QObject::tr("Dir View"));

    QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Expanding);
    sizePolicy.setHorizontalStretch(0);
    sizePolicy.setVerticalStretch(0);
    sizePolicy.setHeightForWidth(fileTree.sizePolicy().hasHeightForWidth());
    fileTree.setSizePolicy(sizePolicy);
    fileTree.setMaximumSize(QSize(300, 16777215));

    //定义QListWidget对象
    //设置QListWidget的显示模式
    imageList.setViewMode(QListView::IconMode);
    //设置QListWidget中单元项的图片大小
    imageList.setIconSize(sizeIconLast);
    //设置QListWidget中单元项的间距
    imageList.setSpacing(10);
    //设置自动适应布局调整（Adjust适应，Fixed不适应），默认不适应
    imageList.setResizeMode(QListWidget::Adjust);
    //设置不能移动
    imageList.setMovement(QListWidget::Static);

    //显示QListWidget
    imageList.show();

    m_btnhidelist.setFixedSize(13,55);
    setlistHideStyle();

    //将Tree 加入主窗口
    QHBoxLayout* hLayout = new QHBoxLayout();
    hLayout->setSpacing(0);
    hLayout->addWidget(&fileTree);
    hLayout->addWidget(&m_btnhidelist);
    hLayout->addWidget(&imageList);

    QGridLayout* gridLayout = new QGridLayout(this);
    gridLayout->setSpacing(6);
    gridLayout->setContentsMargins(11, 11, 11, 11);
    gridLayout->setObjectName(QStringLiteral("gridLayout"));
    gridLayout->addLayout(hLayout,0,0,1,1);

    this->setLayout(gridLayout);
}

void PageSelectImage::setlistShowStyle()
{
    m_btnhidelist.setStyleSheet("QPushButton{border-image:url(:/image/middlewidget/btn_listshow (1).png)}"
                                 "QPushButton:hover{border-image:url(:/image/middlewidget/btn_listshow (2).png)}"
                                 "QPushButton:pressed{border-image:url(:/image/middlewidget/btn_listshow (3).png)}");
}

void PageSelectImage::setlistHideStyle()
{
    m_btnhidelist.setStyleSheet("QPushButton{border-image:url(:/image/middlewidget/btn_listhide (1).png)}"
                                 "QPushButton:hover{border-image:url(:/image/middlewidget/btn_listhide (2).png)}"
                                 "QPushButton:pressed{border-image:url(:/image/middlewidget/btn_listhide (3).png)}");
}

bool PageSelectImage::LoadFloder(QString path) //载入文件夹
{
    bool bIsFloder = QDir().exists(path);
    bool bIsFile = QFile().exists(path);

    //先确认路径是否有效
    if(!bIsFloder && !bIsFile)
    {
        return false;
    }

    QVector<QString> vecFilePath;

    if(bIsFloder)
    {
        //一级目录下寻找 souisrc 文件载入
        QDir dir(path);
        QFileInfoList file_list = dir.entryInfoList(QDir::Files | QDir::Hidden | QDir::NoSymLinks);

        foreach(QFileInfo finfo ,file_list)
        {
           if(finfo.suffix()=="png" || finfo.suffix()=="bmp")
           {
               vecFilePath.push_back(finfo.absoluteFilePath());
           }
        }
    }

    if(bIsFile)
        vecFilePath.push_back(path);

    imageList.clear();
    vecLastFilePath = vecFilePath;
    for(auto tmp : vecFilePath)
    {
        //定义QListWidgetItem对象
        QListWidgetItem *imageItem = new QListWidgetItem;
        //为单元项设置属性
        imageItem->setIcon(QIcon(tmp));
        //imageItem->setText(tr("Browse"));
        //重新设置单元项图片的宽度和高度
        imageItem->setSizeHint(sizeIconLast);
        //将单元项添加到QListWidget中
        imageList.addItem(imageItem);
    }

    return true;
}



bool PageSelectImage::ScaleImageListOnce(bool bAdding)   //缩放列表一次， bAdding 表示是否增加大小
{
    if(bAdding)
        sizeIconLast += QSize(10,10);
    else
        sizeIconLast -= QSize(10,10);

    sizeIconLast.rheight() =  sizeIconLast.rheight() < 30 ? 30: (sizeIconLast.rheight() > 400 ? 400:sizeIconLast.rheight());
    sizeIconLast.rwidth() =  sizeIconLast.rwidth() < 30 ? 30: (sizeIconLast.rwidth() > 400 ? 400:sizeIconLast.rwidth());

    imageList.setIconSize(sizeIconLast);

    int size = imageList.count();
    for(int i = 0; i < size; i++)
        imageList.item(i)->setSizeHint(sizeIconLast);


    return true;
}

void PageSelectImage::showSelectedImg(const QModelIndex &index)
{
    QString path = index.data().toString();
    QModelIndex ParentIndex = index.parent();
    while(ParentIndex.isValid())
    {
        path = ParentIndex.data().toString() + "\\" + path;
        ParentIndex = ParentIndex.parent();
    }
    LoadFloder(path);
}

void PageSelectImage::slot_btnclicked()
{
    if(fileTree.isHidden())//如果是隐藏的
    {
        fileTree.show();
        setlistHideStyle();
    }
    else
    {
        fileTree.hide();
        setlistShowStyle();
    }
}
