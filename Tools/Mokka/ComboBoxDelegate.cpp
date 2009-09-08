#include "ComboBoxDelegate.h"

#include <QtGui>

void ComboBoxDelegate::commitAndCloseEditor()
{
  QComboBox* editor = qobject_cast<QComboBox*>(sender());
  emit commitData(editor);
  emit closeEditor(editor);
};

ComboBoxDelegate::ComboBoxDelegate(QObject* parent)
: QItemDelegate(parent)
{};

QWidget* ComboBoxDelegate::createEditor(QWidget* parent,
                                       const QStyleOptionViewItem& /* option */,
                                       const QModelIndex& /* index */) const
{
  QComboBox* editor = new QComboBox(parent);
  editor->setMaxCount(this->m_List.count());
  editor->insertItems(0, this->m_List);
  connect(editor, SIGNAL(activated(int)), this, SLOT(commitAndCloseEditor()));
  return editor;
};

void ComboBoxDelegate::setEditorData(QWidget* editor, const QModelIndex& index) const
{
  QComboBox* cb = qobject_cast<QComboBox*>(editor);
  cb->setCurrentIndex(cb->findText(index.data(Qt::DisplayRole).toString()));
};

void ComboBoxDelegate::setModelData(QWidget* editor, QAbstractItemModel* model,
                                        const QModelIndex& index) const
{
  QComboBox* cb = qobject_cast<QComboBox*>(editor);
  model->setData(index, cb->currentText(), Qt::EditRole);
};

void ComboBoxDelegate::updateEditorGeometry(QWidget *editor,
                                                const QStyleOptionViewItem& option,
                                                const QModelIndex& /* index */) const
{
  editor->setGeometry(option.rect);
};