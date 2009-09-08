#ifndef ComboBoxDelegate_h
#define ComboBoxDelegate_h

#include <QItemDelegate>

class ComboBoxDelegate : public QItemDelegate
{
  Q_OBJECT
public:
  ComboBoxDelegate(QObject* parent = 0);
  
  QWidget* createEditor(QWidget* parent, const QStyleOptionViewItem& option,
                        const QModelIndex& index) const;
  
  void setEditorData(QWidget* editor, const QModelIndex& index) const;
  void setModelData(QWidget* editor, QAbstractItemModel* model,
                    const QModelIndex& index) const;
  
  void updateEditorGeometry(QWidget* editor, const QStyleOptionViewItem& option,
                            const QModelIndex& index) const;
  
  void setList(const QStringList& l) {this->m_List = l;};
  const QStringList& list() const {return this->m_List;};
  
private slots:
  void commitAndCloseEditor();

private:
  QStringList m_List;
};

#endif // ComboBoxDelegate_h