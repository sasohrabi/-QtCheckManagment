/*
Name: CommonFiles
Version: 1.5.0
Programmer: Aleksey Osipov
e-mail: aliks-os@ukr.net
2010-2014
*/

#include <QColorDialog>
#include "qtexteditex.h"
#include "ui_qtexteditex.h"
#include <QDebug>

QTextEditEx::QTextEditEx(QWidget *parent) : QWidget(parent), m_ui(new Ui::QTextEditEx) {
    m_ui->setupUi(this);
    setupTextActions();
    fontChanged(m_ui->textEdit->font());
    alignmentChanged(m_ui->textEdit->alignment());
    m_ui->textEdit->setFocus();
    textEdit = m_ui->textEdit;
    QButtonGroup *btnGroup = new QButtonGroup(this);
    btnGroup->addButton(m_ui->btnAlignCenter);
    btnGroup->addButton(m_ui->btnAlignJustify);
    btnGroup->addButton(m_ui->btnAlignLeft);
    btnGroup->addButton(m_ui->btnAlignRight);
    QObject::connect(m_ui->textEdit, SIGNAL(currentCharFormatChanged(const QTextCharFormat &)),
            this, SLOT(currentCharFormatChanged(const QTextCharFormat &)));
    QObject::connect(m_ui->textEdit, SIGNAL(cursorPositionChanged()), this, SLOT(cursorPositionChanged()));
    QObject::connect(m_ui->btnCut, SIGNAL(clicked()), m_ui->textEdit, SLOT(cut()));
    QObject::connect(m_ui->btnCopy, SIGNAL(clicked()), m_ui->textEdit, SLOT(copy()));
    QObject::connect(m_ui->btnPaste, SIGNAL(clicked()), m_ui->textEdit, SLOT(paste()));
    QObject::connect(m_ui->btnTextDirection, SIGNAL(clicked()), this, SLOT(textDirection()));

//    m_ui->textEdit->document()->defaultTextOption().setTextDirection(QApplication::layoutDirection());
//    QTextCursor cursor = m_ui->textEdit->textCursor();
//    QTextBlockFormat blockFmt = cursor.blockFormat();
//    if (QApplication::layoutDirection() == Qt::RightToLeft) {
//        blockFmt.setLayoutDirection(Qt::RightToLeft);
//    } else {
//        blockFmt.setLayoutDirection(Qt::LeftToRight);
//    }
//    cursor.setBlockFormat(blockFmt);

//    qDebug()<<m_ui->textEdit->document()->defaultTextOption().textDirection();
//    qDebug()<<m_ui->textEdit->toHtml();
}

void QTextEditEx::textDirection() {
    QTextCursor cursor = m_ui->textEdit->textCursor();
    QTextBlockFormat blockFmt = cursor.blockFormat();

    QTextOption topt = m_ui->textEdit->document()->defaultTextOption();
    if (m_ui->btnTextDirection->isChecked()) {
        topt.setTextDirection(Qt::RightToLeft);
        blockFmt.setLayoutDirection(Qt::RightToLeft);
    } else {
        topt.setTextDirection(Qt::LeftToRight);
        blockFmt.setLayoutDirection(Qt::LeftToRight);
    }
    m_ui->textEdit->document()->setDefaultTextOption(topt);
    cursor.setBlockFormat(blockFmt);
}

QTextEditEx::~QTextEditEx() {
    delete m_ui;
}

void QTextEditEx::setupTextActions() {
    QObject::connect(m_ui->btnTextBold, SIGNAL(clicked()), this, SLOT(textBold()));
    QObject::connect(m_ui->btnTextItalic, SIGNAL(clicked()), this, SLOT(textItalic()));
    QObject::connect(m_ui->btnUnderline, SIGNAL(clicked()), this, SLOT(textUnderline()));
    QObject::connect(m_ui->btnAlignCenter, SIGNAL(clicked()), this, SLOT(textAlign()));
    QObject::connect(m_ui->btnAlignJustify, SIGNAL(clicked()), this, SLOT(textAlign()));
    QObject::connect(m_ui->btnAlignLeft, SIGNAL(clicked()), this, SLOT(textAlign()));
    QObject::connect(m_ui->btnAlignRight, SIGNAL(clicked()), this, SLOT(textAlign()));
    QObject::connect(m_ui->btnTextColor, SIGNAL(clicked()), this, SLOT(textColor()));

    m_ui->comboStyle->addItem(tr("Standard"));
    m_ui->comboStyle->addItem(tr("Bullet List (Disc)"));
    m_ui->comboStyle->addItem(tr("Bullet List (Circle)"));
    m_ui->comboStyle->addItem(tr("Bullet List (Square)"));
    m_ui->comboStyle->addItem(tr("Ordered List (Decimal)"));
    m_ui->comboStyle->addItem(tr("Ordered List (Alpha lower)"));
    m_ui->comboStyle->addItem(tr("Ordered List (Alpha upper)"));
    QObject::connect(m_ui->comboStyle, SIGNAL(activated(int)), this, SLOT(textStyle(int)));

    QObject::connect(m_ui->comboFont, SIGNAL(activated(const QString &)), this, SLOT(textFamily(const QString &)));

    m_ui->comboSize->clear();
    QFontDatabase db;
    foreach(int size, db.standardSizes())
        m_ui->comboSize->addItem(QString::number(size));

    QObject::connect(m_ui->comboSize, SIGNAL(activated(const QString &)), this, SLOT(textSize(const QString &)));
    m_ui->comboSize->setCurrentIndex(m_ui->comboSize->findText(QString::number(QApplication::font().pointSize())));
}

void QTextEditEx::textAlign() {
    if (sender()->objectName() == "btnAlignLeft") {
        m_ui->textEdit->setAlignment(Qt::AlignLeft | Qt::AlignAbsolute);
    } else if (sender()->objectName() == "btnAlignCenter") {
        m_ui->textEdit->setAlignment(Qt::AlignHCenter);
    } else if (sender()->objectName() == "btnAlignRight") {
        m_ui->textEdit->setAlignment(Qt::AlignRight | Qt::AlignAbsolute);
    } else if (sender()->objectName() == "btnAlignJustify") {
        m_ui->textEdit->setAlignment(Qt::AlignJustify);
    }
}

void QTextEditEx::textBold() {
    QTextCharFormat fmt;
    fmt.setFontWeight(m_ui->btnTextBold->isChecked() ? QFont::Bold : QFont::Normal);
    mergeFormatOnWordOrSelection(fmt);
}

void QTextEditEx::textUnderline() {
    QTextCharFormat fmt;
    fmt.setFontUnderline(m_ui->btnUnderline->isChecked());
    mergeFormatOnWordOrSelection(fmt);
}

void QTextEditEx::textItalic() {
    QTextCharFormat fmt;
    fmt.setFontItalic(m_ui->btnTextItalic->isChecked());
    mergeFormatOnWordOrSelection(fmt);
}

void QTextEditEx::textStyle(int styleIndex) {
    QTextCursor cursor = m_ui->textEdit->textCursor();

    if (styleIndex != 0) {
        QTextListFormat::Style style = QTextListFormat::ListDisc;

        switch (styleIndex) {
            default:
            case 1:
                style = QTextListFormat::ListDisc;
                break;
            case 2:
                style = QTextListFormat::ListCircle;
                break;
            case 3:
                style = QTextListFormat::ListSquare;
                break;
            case 4:
                style = QTextListFormat::ListDecimal;
                break;
            case 5:
                style = QTextListFormat::ListLowerAlpha;
                break;
            case 6:
                style = QTextListFormat::ListUpperAlpha;
                break;
        }

        cursor.beginEditBlock();

        QTextBlockFormat blockFmt = cursor.blockFormat();

        QTextListFormat listFmt;

        if (cursor.currentList()) {
            listFmt = cursor.currentList()->format();
        } else {
            listFmt.setIndent(blockFmt.indent() + 1);
            blockFmt.setIndent(0);
            cursor.setBlockFormat(blockFmt);
        }

        listFmt.setStyle(style);

        cursor.createList(listFmt);

        cursor.endEditBlock();
    } else {
        // ####
        QTextBlockFormat bfmt;
        bfmt.setObjectIndex(-1);
        cursor.mergeBlockFormat(bfmt);
    }
}

void QTextEditEx::textFamily(const QString &f) {
    QTextCharFormat fmt;
    fmt.setFontFamily(f);
    mergeFormatOnWordOrSelection(fmt);
}

void QTextEditEx::textSize(const QString &p) {
    qreal pointSize = p.toFloat();
    if (p.toFloat() > 0) {
        QTextCharFormat fmt;
        fmt.setFontPointSize(pointSize);
        mergeFormatOnWordOrSelection(fmt);
    }
}

void QTextEditEx::mergeFormatOnWordOrSelection(const QTextCharFormat &format) {
    QTextCursor cursor = m_ui->textEdit->textCursor();
    if (!cursor.hasSelection())
        cursor.select(QTextCursor::WordUnderCursor);
    cursor.mergeCharFormat(format);
    m_ui->textEdit->mergeCurrentCharFormat(format);
}

void QTextEditEx::alignmentChanged(Qt::Alignment a) {
    if (a & Qt::AlignLeft) {
        m_ui->btnAlignLeft->setChecked(true);
        m_ui->btnAlignCenter->setChecked(false);
        m_ui->btnAlignRight->setChecked(false);
        m_ui->btnAlignJustify->setChecked(false);
    } else if (a & Qt::AlignHCenter) {
        m_ui->btnAlignLeft->setChecked(false);
        m_ui->btnAlignCenter->setChecked(true);
        m_ui->btnAlignRight->setChecked(false);
        m_ui->btnAlignJustify->setChecked(false);
    } else if (a & Qt::AlignRight) {
        m_ui->btnAlignLeft->setChecked(false);
        m_ui->btnAlignCenter->setChecked(false);
        m_ui->btnAlignRight->setChecked(true);
        m_ui->btnAlignJustify->setChecked(false);
    } else if (a & Qt::AlignJustify) {
        m_ui->btnAlignLeft->setChecked(false);
        m_ui->btnAlignCenter->setChecked(false);
        m_ui->btnAlignRight->setChecked(false);
        m_ui->btnAlignJustify->setChecked(true);
    }
}

void QTextEditEx::cursorPositionChanged() {
    alignmentChanged(m_ui->textEdit->alignment());

    QTextBlockFormat blockFmt = m_ui->textEdit->textCursor().blockFormat();
    if (blockFmt.layoutDirection() == Qt::RightToLeft)
        m_ui->btnTextDirection->setChecked(true);
    else
        m_ui->btnTextDirection->setChecked(false);
}

void QTextEditEx::fontChanged(const QFont &f) {
    m_ui->comboFont->setCurrentIndex(m_ui->comboFont->findText(QFontInfo(f).family()));
    m_ui->comboSize->setCurrentIndex(m_ui->comboSize->findText(QString::number(f.pointSize())));
    m_ui->btnTextBold->setChecked(f.bold());
    m_ui->btnTextItalic->setChecked(f.italic());
    m_ui->btnUnderline->setChecked(f.underline());
}

void QTextEditEx::currentCharFormatChanged(const QTextCharFormat &format) {
    fontChanged(format.font());
    //colorChanged(format.foreground().color());
}

void QTextEditEx::textColor() {
    QColor color;
    QColorDialog *dlg = new QColorDialog(this);
    if (dlg->exec() == QDialog::Accepted) {
        color = dlg->selectedColor();
    } else return;
    QTextCharFormat fmt;
    fmt.setForeground( QBrush( color ) );
    mergeFormatOnWordOrSelection(fmt);
}
