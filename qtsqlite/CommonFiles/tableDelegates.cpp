/***********************
*   Aleksey Osipov     *
*  aliks-os@yandex.ru  *
*     2011-2012        *
************************/

#include "tableDelegates.h"
#include <QtWidgets>

//Описываем делегат с проверкой на числа
DigDelegate::DigDelegate(QObject *parent, QString type) : QItemDelegate(parent),
                            m_type(type) {
    QObject::connect(this, SIGNAL(closeEditor(QWidget *, QAbstractItemDelegate::EndEditHint)),
                     this, SLOT(editorClose_(QWidget *, QAbstractItemDelegate::EndEditHint)));
}

QWidget *DigDelegate::createEditor(QWidget *parent,
                                    const QStyleOptionViewItem& option,
                                    const QModelIndex& index ) const {
    return QItemDelegate::createEditor(parent, option,index);
}

void DigDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const {
    QItemDelegate::setEditorData(editor,index);
}

void DigDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex& index) const {
    QLineEdit *le = qobject_cast<QLineEdit*>(editor);
    QString value = le->text();
    int pos = 0;

    if (m_type == "DBL") {
        QDoubleValidator *dv = new QDoubleValidator(le);
        QValidator::State state = dv->validate(value,pos);
        if (state != QValidator::Acceptable) {
            return;
        }
    }
    if (m_type == "INT") {
        QIntValidator *iv = new QIntValidator(le);
        QValidator::State state = iv->validate(value,pos);
        if (state != QValidator::Acceptable) {
            return;
        }
    }
    model->setData(index, value);
}

void DigDelegate::updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex& /* index */) const {
    editor->setGeometry(option.rect);
}

void DigDelegate::editorClose_(QWidget *editor, QAbstractItemDelegate::EndEditHint hint) {
    Q_UNUSED(editor);
    Q_UNUSED(hint);
    emit editorClose(this);
}

void DigDelegate::commitAndCloseEditor() {
    QWidget *editor = qobject_cast<QWidget *>(sender());
    emit commitData(editor);
    emit closeEditor(editor);
}

//Описываем класс делегата-календаря
CalendarDelegate::CalendarDelegate(bool calpopup, QObject *parent) : QItemDelegate(parent), m_calpopup(calpopup) {
}

QWidget *CalendarDelegate::createEditor(QWidget *parent,
                                    const QStyleOptionViewItem& /* option */,
                                    const QModelIndex& /* index */) const {
    QDateEdit *editor = new QDateEdit(parent);
    editor->setCalendarPopup(m_calpopup);
    editor->installEventFilter(const_cast<CalendarDelegate*>(this));
    connect(editor, SIGNAL(editingFinished()), this, SLOT(commitAndCloseEditor()));
    return editor;
}

void CalendarDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const {
    QDate value = index.model()->data(index, Qt::EditRole).toDate();
    QDateEdit *de = static_cast<QDateEdit*>(editor);
    if (value.year() > 2007) de->setDate(value);
    else de->setDate(QDate::currentDate());
}

void CalendarDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex& index) const {
    QDateEdit *de = static_cast<QDateEdit*>(editor);
    de->interpretText();
    QDate value = de->date();
    model->setData(index, value);
}

void CalendarDelegate::updateEditorGeometry(QWidget *editor,
                                        const QStyleOptionViewItem &option,
                                        const QModelIndex& /* index */) const {
    editor->setGeometry(option.rect);
}

void CalendarDelegate::commitAndCloseEditor() {
    QDateEdit *editor = qobject_cast<QDateEdit *>(sender());
    emit commitData(editor);
    emit closeEditor(editor);
}

//Описываем класс делегата-спин-редактора
SpinDelegate::SpinDelegate(double min, double max, double step, int precision, QObject *parent)
                                 : QItemDelegate(parent),
                                   m_min(min),
                                   m_max(max),
                                   m_step(step),
                                   m_precision(precision) {
}

QWidget *SpinDelegate::createEditor(QWidget *parent,
                                     const QStyleOptionViewItem& /* option */,
                                     const QModelIndex& /* index */) const {
    QDoubleSpinBox *editor = new QDoubleSpinBox(parent);
    editor->setMinimum(m_min);
    editor->setMaximum(m_max);
    editor->setDecimals(m_precision);
    editor->setSingleStep(m_step);
    editor->installEventFilter(const_cast<SpinDelegate*>(this));
    QObject::connect(editor, SIGNAL(editingFinished()), this, SLOT(commitAndCloseEditor()));
    return editor;
}

void SpinDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const {
    double value = index.model()->data(index, Qt::EditRole).toDouble();
    QDoubleSpinBox *dsb = static_cast<QDoubleSpinBox*>(editor);
    dsb->setValue(value);
}

void SpinDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex& index) const {
    QDoubleSpinBox *dsb = static_cast<QDoubleSpinBox*>(editor);
    dsb->interpretText();
    double value = dsb->value();
    model->setData(index, value);
}

void SpinDelegate::updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex& /* index */) const {
    editor->setGeometry(option.rect);
}

void SpinDelegate::commitAndCloseEditor() {
    QDoubleSpinBox *editor = qobject_cast<QDoubleSpinBox *>(sender());
    emit commitData(editor);
    emit closeEditor(editor);
}

//Описываем класс делегата-редактора с кнопкой
BtnDelegate::BtnDelegate(QObject *parent) : QItemDelegate(parent) {

}

QWidget* BtnDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem& /* option */, const QModelIndex &/* index */) const {
    QWidget *editor = new QWidget(parent);
    //editor->setStyleSheet("background-color: red; border-top-color: white; border-bottom-color: red;");
    QHBoxLayout *h  = new QHBoxLayout();
    QPushButton *btn = new QPushButton("...", editor);
    btn->setMaximumWidth(20);
    QLineEdit *dd = new QLineEdit(editor);
    QLabel *lb = new QLabel(editor);
    lb->setVisible(false);
    dd->setStyleSheet("background-color: white; border-top-color: white; border-bottom-color: red;");
    h->addWidget(dd);
    h->addWidget(lb);
    h->addWidget(btn);
    h->setContentsMargins(0,0,0,0);
    h->setSpacing(0);
    editor->setLayout(h);
    //QTableWidget *table = qobject_cast<QTableWidget *>(parent->parent());
    QObject::disconnect(btn, SIGNAL(clicked()), this, SIGNAL(clicked()));
    QObject::disconnect(this, SIGNAL(closeEditor(QWidget *, QAbstractItemDelegate::EndEditHint)),
                        this, SLOT(editorClose_(QWidget *, QAbstractItemDelegate::EndEditHint)));
    QObject::connect(btn, SIGNAL(clicked()), this, SIGNAL(clicked()));
    QObject::connect(this, SIGNAL(closeEditor(QWidget *, QAbstractItemDelegate::EndEditHint)),
                     this, SLOT(editorClose_(QWidget *, QAbstractItemDelegate::EndEditHint)));
    //QObject::connect(table, SIGNAL(currentItemChanged(QTableWidgetItem *, QTableWidgetItem *)), this, SLOT( currentItemChanged_(QTableWidgetItem *, QTableWidgetItem *) ) );
    editor->setFocusProxy(dd);
    return editor;
}

void BtnDelegate::currentItemChanged_( QTableWidgetItem * current, QTableWidgetItem * previous) {
    emit editorClose(this);
}

void BtnDelegate::editorClose_(QWidget *editor, QAbstractItemDelegate::EndEditHint hint) {
    Q_UNUSED(editor);
    Q_UNUSED(hint);
    emit editorClose(this);
}

void BtnDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const {
    QString value = index.model()->data(index, Qt::EditRole).toString();    
    QLineEdit *le = editor->findChild<QLineEdit *>();
    le->setText(value);
    QLabel *lb = editor->findChild<QLabel *>();
    lb->setText("");
    if (index.model()->data(index, Qt::DecorationRole).type() == QVariant::Icon) {
        le->setReadOnly(true);
        le->setVisible(false);
    }
    if (index.model()->data(index, Qt::DecorationRole) == 1) {
        le->setVisible(false);
        lb->setVisible(true);
    }
}

void BtnDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex& index) const {
    QLineEdit *le = editor->findChild<QLineEdit *>();
    model->setData(index, le->text());    
}

void BtnDelegate::updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex& /* index */) const {
    editor->setGeometry(option.rect);
}

void BtnDelegate::commitAndCloseEditor() {
    QLineEdit *editor = qobject_cast<QLineEdit *>(sender());
    emit commitData(editor);
    emit closeEditor(editor);
}
