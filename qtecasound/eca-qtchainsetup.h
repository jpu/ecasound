#ifndef INCLUDED_QE_CHAINSETUP_H
#define INCLUDED_QE_CHAINSETUP_H

#include <vector>

#include <qwidget.h>
#include <qlayout.h>
#include <qlistview.h>

class AUDIO_IO;
class ECA_CONTROL;
class ECA_CHAINSETUP;
class QEButtonRow;
class QEChain;

/**
 * Qt widget representing an ecasound chainsetup.
 */
class QEChainsetup : public QWidget
{
  Q_OBJECT
public:
  QEChainsetup (ECA_CONTROL* econtrol, QWidget *parent = 0, const char *name = 0);
  
public slots:
 void update_chain_list(void);
 void update_chain_list_clean(void);

 // --
 void button_add_file(void);
 void button_remove_file(void);
/*   void init_wave_edit(void); */
 // --
 void button_add_chain(void);
 void button_remove_chain(void);
 void button_chain_muting(void);
 void button_chain_bypass(void);

private slots:
 void not_implemented(void);

signals:
 void widget_closed(void);

protected:

  void closeEvent(QCloseEvent *);
  void timerEvent(QTimerEvent *);

 private:

  void init_chain_list(void);
  void init_buttons(void);

  QBoxLayout* top_layout_repp;
  QEButtonRow* buttons_repp;

  ECA_CONTROL* ctrl_repp;
  QListView* chain_list_repp;
};

#endif
