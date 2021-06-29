#include "comboboxdelegate.h"
#include <QComboBox>

ComboBox::ComboBox(QObject *parent) :
    QItemDelegate(parent)
{

}

QWidget *ComboBox::createEditor(QWidget *parent,
                                const QStyleOptionViewItem &option,
                                const QModelIndex &index) const
{
    QComboBox *combobox = new QComboBox(parent);

    if (index.column() == 3)
	{
        combobox->addItem("IPS");
        combobox->addItem("TFT");
        combobox->addItem("OLED");
        combobox->addItem("TN");
    }

    (void)option;
    (void)index;

    return combobox;
}

void ComboBox::setEditorData(QWidget *editor,
                             const QModelIndex &index) const
{
    QComboBox *combobox = static_cast<QComboBox *>(editor);
    combobox->setCurrentText(index.data().toString());
}

void ComboBox::setModelData(QWidget *editor,
                            QAbstractItemModel *model,
                            const QModelIndex &index) const
{
    QComboBox *combobox = static_cast<QComboBox *>(editor);
    model->setData(index, combobox->currentText());
}

void ComboBox::updateEditorGeometry(QWidget *editor,
                                    const QStyleOptionViewItem &option,
                                    const QModelIndex &index) const
{
    editor->setGeometry(option.rect);
    (void)index;
}
