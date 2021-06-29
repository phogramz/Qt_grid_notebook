#ifndef ComboBox_H
#define ComboBox_H

#include <QItemDelegate>


/*!
 \brief Класс ComboBox предназначен для добавления делегата в виде комбинированного списка в таблицу.
 */
class ComboBox : public QItemDelegate
{
public:
	explicit ComboBox(QObject *parent = nullptr);

protected:
    /*!
     * \brief createEditor
     *
     * Метод создающий виджет-редактор
     * \param parent
     * \param option
     * \param index
     * \return
     */
    virtual QWidget * createEditor(QWidget *parent,
                                   const QStyleOptionViewItem &option,
                                   const QModelIndex &index) const override;
    /*!
     * \brief setEditorData
     *
     * Метод передающий значение из таблицы в виджет-редактор
     * \param editor
     * \param index
     */
    virtual void setEditorData(QWidget *editor,
                               const QModelIndex &index) const override;
    /*!
     * \brief setModelData
     *
     * Метод передающий значение из виджета-редактора в таблицу
     * \param editor
     * \param model
     * \param index
     */
    virtual void setModelData(QWidget *editor,
                              QAbstractItemModel *model,
                              const QModelIndex &index) const override;
    /*!
     * \brief updateEditorGeometry
     *
     * Метод определяющий расположение виджета-редактора
     * \param editor
     * \param option
     * \param index
     */
    virtual void updateEditorGeometry(QWidget *editor,
                                      const QStyleOptionViewItem &option,
                                      const QModelIndex &index) const override;

};

#endif // ComboBox_H
