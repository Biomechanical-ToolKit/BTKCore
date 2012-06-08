/* 
 * The Biomechanical ToolKit
 * Copyright (c) 2009-2012, Arnaud Barré
 * All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 
 *     * Redistributions of source code must retain the above
 *       copyright notice, this list of conditions and the following
 *       disclaimer.
 *     * Redistributions in binary form must reproduce the above
 *       copyright notice, this list of conditions and the following
 *       disclaimer in the documentation and/or other materials
 *       provided with the distribution.
 *     * Neither the name(s) of the copyright holders nor the names
 *       of its contributors may be used to endorse or promote products
 *       derived from this software without specific prior written
 *       permission.
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 * COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 * ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#include "ModelDockWidget.h"
#include "NewSegmentDialog.h"
#include "LoggerMessage.h"
#include "UserDefined.h"

#include <QPainter>
#include <QFileDialog>
#include <QMouseEvent>
#include <QColorDialog>
#include <QFileInfo>
#include <QMessageBox>

static const QBrush defaultLabelColor = QBrush(QColor(Qt::gray));
static const QBrush displayLabelColor = QBrush(QColor(Qt::black));

ModelDockWidget::ModelDockWidget(QWidget* parent)
: QDockWidget(parent), m_ConfigurationItems(), m_DisplayedIds(), m_RecentColors(5)
{
  this->m_CurrentConfigurationIndex = -1;
  this->mp_Acquisition = 0;
  this->mp_Model = 0;
  this->mp_DownArrowIcon = new QIcon(QString::fromUtf8(":/Resources/Images/disclosureTriangleSmallDownBlack.png"));
  this->mp_RightArrowIcon = new QIcon(QString::fromUtf8(":/Resources/Images/disclosureTriangleSmallRightBlack.png"));
  this->mp_SegmentsIcon = new QIcon(QString::fromUtf8(":/Resources/Images/link2.png"));
  this->mp_MarkersIcon = new QIcon(QString::fromUtf8(":/Resources/Images/markers.png"));
  this->mp_VirtualMarkersIcon = new QIcon(QString::fromUtf8(":/Resources/Images/virtual-markers.png"));
  this->mp_AnalogsIcon = new QIcon(QString::fromUtf8(":/Resources/Images/chart_line.png"));
  this->mp_ModelOutputsIcon = new QIcon(QString::fromUtf8(":/Resources/Images/chart_curve.png"));
  this->mp_ForcePlatesIcon = new QIcon(QString::fromUtf8(":/Resources/Images/forceplate-16.png"));
  this->mp_VideosIcon = new QIcon(QString::fromUtf8(":/Resources/Images/video.png"));
  this->mp_VideoErrorIcon = new QIcon(QString::fromUtf8(":/Resources/Images/video_error.png"));
  
  this->setupUi(this);
   
  // Finalize the UI
  QFont f = this->modelTree->font();
#ifdef Q_OS_MAC
  // TODO: What about the widget's attributes Qt::WA_MacNormalSize, Qt::WA_MacSmallSize	 and Qt::WA_MacMiniSize for mac?
  // TODO: Think to use this dockwidget as a drawer: // this->setWindowFlags(Qt::Drawer); // Need to check the bug http://bugreports.qt.nokia.com/browse/QTBUG-15897
  this->setStyleSheet("QDockWidget {color: white;} QDockWidget::title {background: qlineargradient(spread:pad, x1:0, y1:0, x2:0, y2:1, stop:0 rgba(128, 128, 128, 255), stop:0.5 rgba(0, 0, 0, 255), stop:0.95 rgba(64, 64, 64, 255), stop:1 rgba(200, 200, 200, 255)); text-align: center; padding-left: -25px;}");
  this->modelTree->setAttribute(Qt::WA_MacShowFocusRect, 0);
  
  // A white background looks better when the tree is disabled
  QPalette palette = this->modelTree->viewport()->palette();
  palette.setBrush(QPalette::Disabled, QPalette::Base, Qt::white);
  this->modelTree->viewport()->setPalette(palette);
  
  f.setPointSize(10);
  this->modelTree->setFont(f);
  this->propertiesButton->setFont(f);
  //  - Segment page
  static_cast<QGridLayout*>(this->segmentPage->layout())->setVerticalSpacing(6);
  this->segmentLabelLabel->setFont(f);
  this->segmentLabelEdit->setFont(f);
  this->segmentColorLabel->setFont(f);
  this->segmentDescLabel->setFont(f);
  this->segmentDescEdit->setFont(f);
  this->segmentLinkLabel->setFont(f);
  this->segmentEditLinksButton->setFont(f);
  this->segmentOptionsLabel->setFont(f);
  //  - Marker page
  static_cast<QGridLayout*>(this->markerPage->layout())->setVerticalSpacing(3);
  this->markerLabelLabel->setFont(f);
  this->markerLabelEdit->setFont(f);
  this->markerRadiusLabel->setFont(f);
  // FIX: Setting the font for the spinbox has no effect with Mac
  // this->markerRadiusSpinBox->setFont(f);
  this->markerRadiusSpinBox->setStyleSheet("QDoubleSpinBox {font-size: 10px;};");
  this->markerColorLabel->setFont(f);
  this->markerDescLabel->setFont(f);
  this->markerDescEdit->setFont(f);
  this->markerOptionsLabel->setFont(f);
  // - Point page
  static_cast<QGridLayout*>(this->pointPage->layout())->setVerticalSpacing(3);
  this->pointLabelLabel->setFont(f);
  this->pointLabelEdit->setFont(f);
  this->pointDescLabel->setFont(f);
  this->pointDescEdit->setFont(f);
  // - Analog page
  static_cast<QGridLayout*>(this->analogPage->layout())->setVerticalSpacing(3);
  this->analogLabelLabel->setFont(f);
  this->analogLabelEdit->setFont(f);
  this->analogUnitLabel->setFont(f);
  this->analogUnitEdit->setFont(f);
  this->analogGainLabel->setFont(f);
  this->analogGainComboBox->setFont(f);
  this->analogScaleLabel->setFont(f);
  // this->analogScaleSpinBox->setFont(f);
  this->analogScaleSpinBox->setStyleSheet("QDoubleSpinBox {font-size: 10px;};");
  this->analogOffsetLabel->setFont(f);
  //this->analogOffsetSpinBox->setFont(f);
  this->analogOffsetSpinBox->setStyleSheet("QSpinBox {font-size: 10px;};");
  this->analogDescLabel->setFont(f);
  this->analogDescEdit->setFont(f);
  // - Video page
  static_cast<QGridLayout*>(this->videoPage->layout())->setVerticalSpacing(3);
  this->videoFilenameLabel->setFont(f);
  this->videoFilenameEdit->setFont(f);
  this->videoDelayLabel->setFont(f);
  this->videoDelaySpinBox->setStyleSheet("QDoubleSpinBox {font-size: 10px;};");
#endif
  QMenu* modelConfigurationMenu = new QMenu(this);
  this->mp_NewConfiguration = new QAction(tr("New configuration"), this);
  this->mp_LoadConfiguration = new QAction(tr("Load configuration"), this);
  this->mp_SaveConfiguration = new QAction(tr("Save configuration"), this);
  this->mp_SaveConfiguration->setEnabled(false);
  this->mp_RemoveConfiguration = new QAction(tr("Remove configuration"), this);
  this->mp_RemoveConfiguration->setEnabled(false);
  this->mp_DeselectConfiguration = new QAction(tr("Deselect configuration"), this);
  this->mp_DeselectConfiguration->setEnabled(false);
  this->mp_ClearConfigurations = new QAction(tr("Clear configurations"), this);
  this->mp_NewSegment = new QAction(tr("New Segment"), this);
  this->mp_HideSelectedSegments = new QAction(tr("Hide selected segments"), this);
  this->mp_UnhideSelectedSegments = new QAction(tr("Unhide selected segments"), this);
  this->mp_SelectAllMarkers = new QAction(tr("Select all markers"), this);
  this->mp_HideSelectedMarkers = new QAction(tr("Hide selected markers"), this);
  this->mp_UnhideSelectedMarkers = new QAction(tr("Unhide selected markers"), this);
  this->mp_TrackSelectedMarkers = new QAction(tr("Track selected markers"), this);
  this->mp_UntrackSelectedMarkers = new QAction(tr("Untrack selected markers"), this);
  this->mp_DeleteSelectedMarkers = new QAction(tr("Delete selected markers"), this);
  this->mp_SelectAllAnalogs = new QAction(tr("Select all analog channels"), this);
  this->mp_SelectAllModelOutputs = new QAction(tr("Select all model outputs"), this);
  this->mp_SelectAllAngles = new QAction(tr("Select all angles"), this);
  this->mp_SelectAllForces = new QAction(tr("Select all forces"), this);
  this->mp_SelectAllMoments = new QAction(tr("Select all moments"), this);
  this->mp_SelectAllPowers = new QAction(tr("Select all powers"), this);
  this->mp_SelectAllScalars = new QAction(tr("Select all scalars"), this);
  this->mp_RemoveSelectedItems = new QAction(tr("Remove selected items"), this);
  // Model configuration menu
  modelConfigurationMenu->addAction(this->mp_NewConfiguration);
  modelConfigurationMenu->addAction(this->mp_LoadConfiguration);
  modelConfigurationMenu->addAction(this->mp_SaveConfiguration);
  modelConfigurationMenu->addAction(this->mp_RemoveConfiguration);
  modelConfigurationMenu->addAction(this->mp_DeselectConfiguration);
  modelConfigurationMenu->addSeparator();
  modelConfigurationMenu->addAction(this->mp_ClearConfigurations);
  this->modelConfigurationButton->setMenu(modelConfigurationMenu);
  // Contextual menu
  QList<QAction*> actions;
  actions.push_back(this->mp_NewSegment);
  actions.push_back(this->mp_HideSelectedSegments);
  actions.push_back(this->mp_UnhideSelectedSegments);
  QAction* sep1 = new QAction(this); sep1->setSeparator(true); actions.push_back(sep1);
  actions.push_back(this->mp_SelectAllMarkers);
  actions.push_back(this->mp_HideSelectedMarkers);
  actions.push_back(this->mp_UnhideSelectedMarkers);
  QAction* sep2 = new QAction(this); sep2->setSeparator(true); actions.push_back(sep2);
  actions.push_back(this->mp_SelectAllAnalogs);
  actions.push_back(this->mp_SelectAllModelOutputs);
  actions.push_back(this->mp_SelectAllAngles);
  actions.push_back(this->mp_SelectAllForces);
  actions.push_back(this->mp_SelectAllMoments);
  actions.push_back(this->mp_SelectAllPowers);
  actions.push_back(this->mp_SelectAllScalars);
  QAction* sep3 = new QAction(this); sep3->setSeparator(true); actions.push_back(sep3);
  actions.push_back(this->mp_RemoveSelectedItems);
  this->modelTree->addActions(actions);
  this->modelTree->setContextMenuPolicy(Qt::ActionsContextMenu);
  // Marker color menu
  QMenu* markerColorMenu = new QMenu(this);
  this->mp_ResetMarkerColorHistory = new QAction(tr("Reset recent colors"), this);
  this->mp_SetMarkerColorInHistory1 = new QAction(tr("Set recent color #1"), this);
  this->mp_SetMarkerColorInHistory2 = new QAction(tr("Set recent color #2"), this);
  this->mp_SetMarkerColorInHistory3 = new QAction(tr("Set recent color #3"), this);
  this->mp_SetMarkerColorInHistory4 = new QAction(tr("Set recent color #4"), this);
  this->mp_SetMarkerColorInHistory5 = new QAction(tr("Set recent color #5"), this);
  markerColorMenu->addAction(this->mp_ResetMarkerColorHistory);
  markerColorMenu->addSeparator();
  markerColorMenu->addAction(this->mp_SetMarkerColorInHistory1);
  markerColorMenu->addAction(this->mp_SetMarkerColorInHistory2);
  markerColorMenu->addAction(this->mp_SetMarkerColorInHistory3);
  markerColorMenu->addAction(this->mp_SetMarkerColorInHistory4);
  markerColorMenu->addAction(this->mp_SetMarkerColorInHistory5);
  this->markerColorButtonMenu->setMenu(markerColorMenu);
  // Tree
  QList<QTreeWidgetItem *> items;
  f = this->modelTree->font();
  f.setBold(true);
  // Header size
  QHeaderView* modelTreeHeader = this->modelTree->header();
  modelTreeHeader->setDefaultSectionSize(26);
  modelTreeHeader->setResizeMode(LabelHeader, QHeaderView::Stretch);
  modelTreeHeader->setResizeMode(VisibleHeader, QHeaderView::Fixed);
  modelTreeHeader->setResizeMode(TrajectoryHeader, QHeaderView::Fixed);
  // Segments
  QTreeWidgetItem* segmentsRoot = new QTreeWidgetItem(QStringList(QString("Segments")));
  segmentsRoot->setFont(LabelHeader, f);
  segmentsRoot->setIcon(LabelHeader, *this->mp_SegmentsIcon);
  segmentsRoot->setFlags(segmentsRoot->flags() & ~Qt::ItemIsSelectable);
  items.append(segmentsRoot);
  // Markers
  QTreeWidgetItem* markersRoot = new QTreeWidgetItem(QStringList(QString("Markers")));
  markersRoot->setFont(LabelHeader, f);
  markersRoot->setIcon(LabelHeader, *this->mp_MarkersIcon);
  markersRoot->setFlags(markersRoot->flags() & ~Qt::ItemIsSelectable);
  items.append(markersRoot);
  // Markers
  QTreeWidgetItem* virtualMarkersRoot = new QTreeWidgetItem(QStringList(QString("Virtual markers")));
  virtualMarkersRoot->setFont(LabelHeader, f);
  virtualMarkersRoot->setIcon(LabelHeader, *this->mp_VirtualMarkersIcon);
  virtualMarkersRoot->setFlags(virtualMarkersRoot->flags() & ~Qt::ItemIsSelectable);
  items.append(virtualMarkersRoot);
  // Analog channels
  QTreeWidgetItem* analogsRoot = new QTreeWidgetItem(QStringList(QString("Analog channels")));
  analogsRoot->setFont(LabelHeader, f);
  analogsRoot->setIcon(LabelHeader, *this->mp_AnalogsIcon);
  analogsRoot->setFlags(analogsRoot->flags() & ~Qt::ItemIsSelectable);
  items.append(analogsRoot);
  // Force plates
  QTreeWidgetItem* forcePlatesRoot = new QTreeWidgetItem(QStringList(QString("Force platform reactions")));
  forcePlatesRoot->setFont(LabelHeader, f);
  forcePlatesRoot->setIcon(LabelHeader, *this->mp_ForcePlatesIcon);
  forcePlatesRoot->setFlags(forcePlatesRoot->flags() & ~Qt::ItemIsSelectable);
  items.append(forcePlatesRoot);
  // Model outputs
  QTreeWidgetItem* modelOutputsRoot = new QTreeWidgetItem(QStringList(QString("Model outputs")));
  modelOutputsRoot->setFont(LabelHeader, f);
  modelOutputsRoot->setIcon(LabelHeader, *this->mp_ModelOutputsIcon);
  modelOutputsRoot->setFlags(modelOutputsRoot->flags() & ~Qt::ItemIsSelectable);
  items.append(modelOutputsRoot);
  // Model videos
  QTreeWidgetItem* videosRoot = new QTreeWidgetItem(QStringList(QString("Videos")));
  videosRoot->setFont(LabelHeader, f);
  videosRoot->setIcon(LabelHeader, *this->mp_VideosIcon);
  videosRoot->setFlags(videosRoot->flags() & ~Qt::ItemIsSelectable);
  items.append(videosRoot);
  // Insert top level items
  this->modelTree->addTopLevelItems(items);
  items.clear();
  // Model outputs: Angles
  QTreeWidgetItem* modelOutputAngles = new QTreeWidgetItem(QStringList(QString("Angles")));
  modelOutputAngles->setFont(LabelHeader, f);
  modelOutputAngles->setIcon(LabelHeader, *this->mp_ModelOutputsIcon);
  modelOutputAngles->setFlags(modelOutputAngles->flags() & ~Qt::ItemIsSelectable);
  items.append(modelOutputAngles);
  // Model outputs: Forces
  QTreeWidgetItem* modelOutputForces = new QTreeWidgetItem(QStringList(QString("Forces")));
  modelOutputForces->setFont(LabelHeader, f);
  modelOutputForces->setIcon(LabelHeader, *this->mp_ModelOutputsIcon);
  modelOutputForces->setFlags(modelOutputForces->flags() & ~Qt::ItemIsSelectable);
  items.append(modelOutputForces);
  // Model outputs: Moments
  QTreeWidgetItem* modelOutputMoments = new QTreeWidgetItem(QStringList(QString("Moments")));
  modelOutputMoments->setFont(LabelHeader, f);
  modelOutputMoments->setIcon(LabelHeader, *this->mp_ModelOutputsIcon);
  modelOutputMoments->setFlags(modelOutputMoments->flags() & ~Qt::ItemIsSelectable);
  items.append(modelOutputMoments);
  // Model outputs: Powers
  QTreeWidgetItem* modelOutputPowers = new QTreeWidgetItem(QStringList(QString("Powers")));
  modelOutputPowers->setFont(LabelHeader, f);
  modelOutputPowers->setIcon(LabelHeader, *this->mp_ModelOutputsIcon);
  modelOutputPowers->setFlags(modelOutputPowers->flags() & ~Qt::ItemIsSelectable);
  items.append(modelOutputPowers);
  // Model outputs: Scalars
  QTreeWidgetItem* modelOutputScalars = new QTreeWidgetItem(QStringList(QString("Scalars")));
  modelOutputScalars->setFont(LabelHeader, f);
  modelOutputScalars->setIcon(LabelHeader, *this->mp_ModelOutputsIcon);
  modelOutputScalars->setFlags(modelOutputScalars->flags() & ~Qt::ItemIsSelectable);
  items.append(modelOutputScalars);
  // Insert model output items
  modelOutputsRoot->addChildren(items);
  // Strech
  this->splitter->setStretchFactor(0, 10);
  
  // Connections
  connect(this->modelConfigurationComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(selectConfiguration(int)));
  connect(this->mp_NewConfiguration, SIGNAL(triggered()), this, SLOT(newConfiguration()));
  connect(this->mp_LoadConfiguration, SIGNAL(triggered()), this, SLOT(loadConfiguration()));
  connect(this->mp_SaveConfiguration, SIGNAL(triggered()), this, SLOT(saveConfiguration()));
  connect(this->mp_RemoveConfiguration, SIGNAL(triggered()), this, SLOT(removeConfiguration()));
  connect(this->mp_DeselectConfiguration, SIGNAL(triggered()), this, SLOT(deselectConfiguration()));
  connect(this->mp_ClearConfigurations, SIGNAL(triggered()), this, SLOT(clearConfigurations()));
  connect(this->mp_NewSegment, SIGNAL(triggered()), this, SIGNAL(segmentCreationRequested()));
  connect(this->mp_HideSelectedSegments, SIGNAL(triggered()), this, SLOT(hideSelectedSegments()));
  connect(this->mp_UnhideSelectedSegments, SIGNAL(triggered()), this, SLOT(unhideSelectedSegments()));
  connect(this->mp_SelectAllMarkers, SIGNAL(triggered()), this, SLOT(selectAllMarkers()));
  connect(this->mp_HideSelectedMarkers, SIGNAL(triggered()), this, SLOT(hideSelectedMarkers()));
  connect(this->mp_UnhideSelectedMarkers, SIGNAL(triggered()), this, SLOT(unhideSelectedMarkers()));
  connect(this->mp_TrackSelectedMarkers, SIGNAL(triggered()), this, SLOT(trackSelectedMarkers()));
  connect(this->mp_UntrackSelectedMarkers, SIGNAL(triggered()), this, SLOT(untrackSelectedMarkers()));
  connect(this->mp_DeleteSelectedMarkers, SIGNAL(triggered()), this, SLOT(removeSelectedItems()));
  connect(this->mp_SelectAllAnalogs, SIGNAL(triggered()), this, SLOT(selectAllAnalogs()));
  connect(this->mp_SelectAllModelOutputs, SIGNAL(triggered()), this, SLOT(selectAllModelOutputs()));
  connect(this->mp_SelectAllAngles, SIGNAL(triggered()), this, SLOT(selectAllAngles()));
  connect(this->mp_SelectAllForces, SIGNAL(triggered()), this, SLOT(selectAllForces()));
  connect(this->mp_SelectAllMoments, SIGNAL(triggered()), this, SLOT(selectAllMoments()));
  connect(this->mp_SelectAllPowers, SIGNAL(triggered()), this, SLOT(selectAllPowers()));
  connect(this->mp_SelectAllScalars, SIGNAL(triggered()), this, SLOT(selectAllScalars()));
  connect(this->mp_RemoveSelectedItems, SIGNAL(triggered()), this, SLOT(removeSelectedItems()));  
  connect(this->propertiesButton, SIGNAL(clicked()), this, SLOT(toggleProperties()));
  connect(this->modelTree, SIGNAL(itemSelectionChanged()), this, SLOT(displayProperties()));
  connect(this->modelTree, SIGNAL(itemSelectionChanged()), this, SLOT(sendSelectedMarkers()));
  connect(this->modelTree, SIGNAL(itemChanged(QTreeWidgetItem*, int)), this, SLOT(sendModifiedMarkersState(QTreeWidgetItem*, int)));
  connect(this->segmentLabelEdit, SIGNAL(editingFinished()), this, SLOT(editSegmentLabel()));
  connect(this->segmentDescEdit, SIGNAL(editingFinished()), this, SLOT(editSegmentsDescription()));
  connect(this->segmentEditLinksButton, SIGNAL(clicked()), this, SIGNAL(segmentEditionRequested()));
  connect(this->segmentVisibilityCheckBox, SIGNAL(stateChanged(int)), this, SLOT(editSegmentsVisibility()));
  connect(this->segmentSurfaceVisibilityCheckBox, SIGNAL(stateChanged(int)), this, SLOT(editSegmentsSurfaceVisibility()));
  connect(this->markerLabelEdit, SIGNAL(editingFinished()), this, SLOT(editMarkerLabel()));
  connect(this->markerDescEdit, SIGNAL(editingFinished()), this, SLOT(editMarkersDescription()));
  connect(this->markerRadiusSlider, SIGNAL(valueChanged(int)), this, SLOT(updateMarkerRadiusSpinBox(int)));
  connect(this->markerRadiusSpinBox, SIGNAL(valueChanged(double)), this, SLOT(updateMarkerRadius(double)));
  connect(this->markerRadiusSpinBox, SIGNAL(editingFinished()), this, SLOT(editMarkersRadius()));
  connect(this->markerRadiusSlider, SIGNAL(sliderReleased()), this, SLOT(editMarkersRadius()));
  connect(this->markerColorButton, SIGNAL(clicked()), this, SLOT(editMarkersColor()));
  connect(this->markerColorRecent1, SIGNAL(clicked()), this, SLOT(setMarkerColorFromRecent1()));
  connect(this->markerColorRecent2, SIGNAL(clicked()), this, SLOT(setMarkerColorFromRecent2()));
  connect(this->markerColorRecent3, SIGNAL(clicked()), this, SLOT(setMarkerColorFromRecent3()));
  connect(this->markerColorRecent4, SIGNAL(clicked()), this, SLOT(setMarkerColorFromRecent4()));
  connect(this->markerColorRecent5, SIGNAL(clicked()), this, SLOT(setMarkerColorFromRecent5()));
  connect(this->mp_ResetMarkerColorHistory, SIGNAL(triggered()), this, SLOT(resetMarkerRecentColors()));
  connect(this->mp_SetMarkerColorInHistory1, SIGNAL(triggered()), this, SLOT(setMarkerRecentColor1()));
  connect(this->mp_SetMarkerColorInHistory2, SIGNAL(triggered()), this, SLOT(setMarkerRecentColor2()));
  connect(this->mp_SetMarkerColorInHistory3, SIGNAL(triggered()), this, SLOT(setMarkerRecentColor3()));
  connect(this->mp_SetMarkerColorInHistory4, SIGNAL(triggered()), this, SLOT(setMarkerRecentColor4()));
  connect(this->mp_SetMarkerColorInHistory5, SIGNAL(triggered()), this, SLOT(setMarkerRecentColor5()));
  connect(this->markerVisibilityCheckBox, SIGNAL(stateChanged(int)), this, SLOT(editMarkersVisibility()));
  connect(this->markerTrajectoryVisibilityCheckBox, SIGNAL(stateChanged(int)), this, SLOT(editMarkersTrajectoryVisibility()));
  connect(this->pointLabelEdit, SIGNAL(editingFinished()), this, SLOT(editPointLabel()));
  connect(this->pointDescEdit, SIGNAL(editingFinished()), this, SLOT(editPointsDescription()));
  connect(this->analogLabelEdit, SIGNAL(editingFinished()), this, SLOT(editAnalogLabel()));
  connect(this->analogUnitEdit, SIGNAL(editingFinished()), this, SLOT(editAnalogsUnit()));
  connect(this->analogGainComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(editAnalogsGain(int)));
  connect(this->analogScaleSpinBox, SIGNAL(editingFinished()), this, SLOT(editAnalogsScale()));
  connect(this->analogOffsetSpinBox, SIGNAL(editingFinished()), this, SLOT(editAnalogsOffset()));
  connect(this->analogDescEdit, SIGNAL(editingFinished()), this, SLOT(editAnalogsDescription()));
  connect(this->videoDelaySlider, SIGNAL(valueChanged(int)), this, SLOT(updateVideoDelaySpinBox(int)));
  connect(this->videoDelaySpinBox, SIGNAL(valueChanged(double)), this, SLOT(updateVideoDelay(double)));
  connect(this->videoDelaySpinBox, SIGNAL(editingFinished()), this, SLOT(editVideosDelay()));
  connect(this->videoDelaySlider, SIGNAL(sliderReleased()), this, SLOT(editVideosDelay()));
  
  this->refresh(); // Disable the "Select *" actions in the contextual menu and hide the items in the tree.
  this->displayProperties(); // Disable the other actions in the contextual menu and set the properties widget to the empty page.
};
  
ModelDockWidget::~ModelDockWidget()
{
  delete this->mp_DownArrowIcon;
  delete this->mp_RightArrowIcon;
  delete this->mp_SegmentsIcon;
  delete this->mp_MarkersIcon;
  delete this->mp_VirtualMarkersIcon;
  delete this->mp_AnalogsIcon;
  delete this->mp_ModelOutputsIcon;
  delete this->mp_ForcePlatesIcon;
  delete this->mp_VideosIcon;
  delete this->mp_VideoErrorIcon;
};

void ModelDockWidget::setAcquisition(Acquisition* acq)
{
  if (this->mp_Acquisition)
    disconnect(this->mp_Acquisition, 0, this, 0);
  this->mp_Acquisition = acq;
  connect(this->mp_Acquisition, SIGNAL(pointLabelChanged(int, QString)), this, SLOT(setPointLabel(int, QString)));
  connect(this->mp_Acquisition, SIGNAL(markersRadiusChanged(QVector<int>, QVector<double>)), this, SLOT(setMarkersRadius(QVector<int>, QVector<double>)));
  connect(this->mp_Acquisition, SIGNAL(markersColorChanged(QVector<int>, QVector<QColor>)), this, SLOT(setMarkersColor(QVector<int>, QVector<QColor>)));
  connect(this->mp_Acquisition, SIGNAL(markersVisibilityChanged(QVector<int>, QVector<bool>)), this, SLOT(setMarkersVisibility(QVector<int>, QVector<bool>)));
  connect(this->mp_Acquisition, SIGNAL(markersTrajectoryVisibilityChanged(QVector<int>, QVector<bool>)), this, SLOT(setMarkersTrajectoryVisibility(QVector<int>, QVector<bool>)));
  connect(this->mp_Acquisition, SIGNAL(markersConfigurationReset(QList<int>, QList<bool>, QList<bool>, QList<double>, QList<QColor>)), this, SLOT(setMarkersConfiguration(QList<int>, QList<bool>, QList<bool>, QList<double>, QList<QColor>)));
  connect(this->mp_Acquisition, SIGNAL(pointsDescriptionChanged(QVector<int>, QVector<QString>)), this, SLOT(setPointsDescription(QVector<int>, QVector<QString>)));
  connect(this->mp_Acquisition, SIGNAL(pointsRemoved(QList<int>, QList<Point*>)), this, SLOT(removePoints(QList<int>, QList<Point*>)));
  connect(this->mp_Acquisition, SIGNAL(pointsInserted(QList<int>, QList<Point*>)), this, SLOT(insertPoints(QList<int>, QList<Point*>)));
  connect(this->mp_Acquisition, SIGNAL(analogLabelChanged(int, QString)), this, SLOT(setAnalogLabel(int, QString)));
  connect(this->mp_Acquisition, SIGNAL(analogsUnitChanged(QVector<int>, QVector<QString>)), this, SLOT(setAnalogsUnit(QVector<int>, QVector<QString>)));
  connect(this->mp_Acquisition, SIGNAL(analogsGainChanged(QVector<int>, QVector<Analog::Gain>)), this, SLOT(setAnalogsGain(QVector<int>, QVector<Analog::Gain>)));
  connect(this->mp_Acquisition, SIGNAL(analogsOffsetChanged(QVector<int>, QVector<int>)), this, SLOT(setAnalogsOffset(QVector<int>, QVector<int>)));
  connect(this->mp_Acquisition, SIGNAL(analogsScaleChanged(QVector<int>, QVector<double>)), this, SLOT(setAnalogsScale(QVector<int>, QVector<double>)));
  connect(this->mp_Acquisition, SIGNAL(analogsDescriptionChanged(QVector<int>, QVector<QString>)), this, SLOT(setAnalogsDescription(QVector<int>, QVector<QString>)));
  connect(this->mp_Acquisition, SIGNAL(analogsRemoved(QList<int>, QList<Analog*>)), this, SLOT(removeAnalogs(QList<int>, QList<Analog*>)));
  connect(this->mp_Acquisition, SIGNAL(analogsInserted(QList<int>, QList<Analog*>)), this, SLOT(insertAnalogs(QList<int>, QList<Analog*>)));
  connect(this->mp_Acquisition, SIGNAL(videosRemoved(QList<int>, QList<Video*>)), this, SLOT(removeVideos(QList<int>, QList<Video*>)));
  connect(this->mp_Acquisition, SIGNAL(videosInserted(QList<int>, QList<Video*>)), this, SLOT(insertVideos(QList<int>, QList<Video*>)));
  connect(this->mp_Acquisition, SIGNAL(videosImported(QList<int>, QList<Video*>)), this, SLOT(importVideos(QList<int>, QList<Video*>)));
  connect(this->mp_Acquisition, SIGNAL(videosDelayChanged(QVector<int>, QVector<qint64>)), this, SLOT(setVideosDelay(QVector<int>, QVector<qint64>)));
  connect(this->mp_Acquisition, SIGNAL(firstFrameChanged(int)), this, SLOT(displayProperties()));
};

void ModelDockWidget::setModel(Model* m)
{
  if (this->mp_Model)
    disconnect(this->mp_Model, 0, this, 0);
  this->mp_Model = m;
  connect(this->mp_Model, SIGNAL(segmentsChanged(QList<int>, QList<Segment*>)), this, SLOT(setSegments(QList<int>, QList<Segment*>)));
  connect(this->mp_Model, SIGNAL(segmentLabelChanged(int, QString)), this, SLOT(setSegmentLabel(int, QString)));
  connect(this->mp_Model, SIGNAL(segmentsColorChanged(QVector<int>, QVector<QColor>)), this, SLOT(setSegmentsColor(QVector<int>, QVector<QColor>)));
  connect(this->mp_Model, SIGNAL(segmentsDescriptionChanged(QVector<int>, QVector<QString>)), this, SLOT(setSegmentsDescription(QVector<int>, QVector<QString>)));
  connect(this->mp_Model, SIGNAL(segmentsVisibilityChanged(QVector<int>, QVector<bool>)), this, SLOT(setSegmentsVisibility(QVector<int>, QVector<bool>)));
  connect(this->mp_Model, SIGNAL(segmentsSurfaceVisibilityChanged(QVector<int>, QVector<bool>)), this, SLOT(setSegmentsSurfaceVisibility(QVector<int>, QVector<bool>)));
  connect(this->mp_Model, SIGNAL(segmentsRemoved(QList<int>, QList<Segment*>)), this, SLOT(removeSegments(QList<int>, QList<Segment*>)));
  connect(this->mp_Model, SIGNAL(segmentsInserted(QList<int>, QList<Segment*>)), this, SLOT(insertSegments(QList<int>, QList<Segment*>)));
};
  
void ModelDockWidget::load()
{
  // Clear the UI
  this->reset();
  
  if (!this->mp_Acquisition)
    return;
    
  // Create the tree
  QTreeWidgetItem* markersRoot = this->modelTree->topLevelItem(MarkersItem);
  QTreeWidgetItem* virtualMarkersRoot = this->modelTree->topLevelItem(VirtualMarkersItem);
  QTreeWidgetItem* modelOutputsRoot = this->modelTree->topLevelItem(ModelOutputsItem);
  QTreeWidgetItem* modelOutputAngles = modelOutputsRoot->child(0);
  QTreeWidgetItem* modelOutputForces = modelOutputsRoot->child(1);
  QTreeWidgetItem* modelOutputMoments = modelOutputsRoot->child(2);
  QTreeWidgetItem* modelOutputPowers = modelOutputsRoot->child(3);
  QTreeWidgetItem* modelOutputScalars = modelOutputsRoot->child(4);
  // - Points
  for (QMap<int, Point*>::const_iterator it = this->mp_Acquisition->points().begin() ; it != this->mp_Acquisition->points().end() ; ++it)
  {
    if (it.value()->type == Point::Marker)
      markersRoot->addChild(this->createMarkerItem(it.value()->label, it.key()));
    else if ((it.value()->type == Point::VirtualMarker) || (it.value()->type == Point::VirtualMarkerForFrame))
      virtualMarkersRoot->addChild(this->createMarkerItem(it.value()->label, it.key(), false));
    else if (it.value()->type == Point::Angle)
      modelOutputAngles->addChild(this->createModelOutputItem(it.value()->label, it.key()));
    else if (it.value()->type == Point::Force)
      modelOutputForces->addChild(this->createModelOutputItem(it.value()->label, it.key()));
    else if (it.value()->type == Point::Moment)
      modelOutputMoments->addChild(this->createModelOutputItem(it.value()->label, it.key()));
    else if (it.value()->type == Point::Power)
      modelOutputPowers->addChild(this->createModelOutputItem(it.value()->label, it.key()));
    else if (it.value()->type == Point::Scalar)
      modelOutputScalars->addChild(this->createModelOutputItem(it.value()->label, it.key()));
  }
  markersRoot->setExpanded(true);
  // - Analogs
  QTreeWidgetItem* analogsRoot = this->modelTree->topLevelItem(AnalogsItem);
  for (QMap<int, Analog*>::const_iterator it = this->mp_Acquisition->analogs().begin() ; it != this->mp_Acquisition->analogs().end() ; ++it)
    analogsRoot->addChild(this->createAnalogItem(it.value()->label, it.key()));
  // - Force platforms
  QTreeWidgetItem* forcePlatesRoot = this->modelTree->topLevelItem(ForcePlatesItem);
  int numFP = this->mp_Acquisition->btkForcePlatforms()->GetItemNumber();
  const int ncfp = 4; // Number of components per forceplate
  for (int i = 0 ; i < numFP ; ++i)
  {
    QTreeWidgetItem* forcePlateItem = new QTreeWidgetItem(QStringList("Force platform #" + QString::number(i+1)), ForcePlateType);
    forcePlateItem->setFont(LabelHeader, forcePlatesRoot->font(LabelHeader));
    forcePlateItem->setFlags(forcePlateItem->flags() & ~Qt::ItemIsSelectable);
    forcePlateItem->setIcon(LabelHeader, *this->mp_ForcePlatesIcon);
    forcePlatesRoot->addChild(forcePlateItem);
    // Force
    QTreeWidgetItem* forcePlateForceItem = new QTreeWidgetItem(QStringList("Force"), ForcePlateType);
    forcePlateForceItem->setIcon(LabelHeader, *this->mp_ModelOutputsIcon);
    forcePlateForceItem->setData(0, ForcePlateId, 65535+i*ncfp+1); // 65535 (to have a unique ID) + FP index * 3 + component index
    forcePlateItem->addChild(forcePlateForceItem);
    // Moment
    QTreeWidgetItem* forcePlateMomentItem = new QTreeWidgetItem(QStringList("Moment"), ForcePlateType);
    forcePlateMomentItem->setIcon(LabelHeader, *this->mp_ModelOutputsIcon);
    forcePlateMomentItem->setData(0, ForcePlateId, 65535+i*ncfp+2);
    forcePlateItem->addChild(forcePlateMomentItem);
    // Position
    QTreeWidgetItem* forcePlatePositionItem = new QTreeWidgetItem(QStringList("Position"), ForcePlateType);
    forcePlatePositionItem->setIcon(LabelHeader, *this->mp_ModelOutputsIcon);
    forcePlatePositionItem->setData(0, ForcePlateId, 65535+i*ncfp+0);
    forcePlatePositionItem->setCheckState(TrajectoryHeader, Qt::Unchecked);
    forcePlateItem->addChild(forcePlatePositionItem);
    // Direction angle
    QTreeWidgetItem* forcePlateDirectionAngleItem = new QTreeWidgetItem(QStringList("Direction angle"), ForcePlateType);
    forcePlateDirectionAngleItem->setIcon(LabelHeader, *this->mp_ModelOutputsIcon);
    forcePlateDirectionAngleItem->setData(0, ForcePlateId, 65535+i*ncfp+3);
    forcePlateItem->addChild(forcePlateDirectionAngleItem);
  }
  // - Videos
  QTreeWidgetItem* videosRoot = this->modelTree->topLevelItem(VideosItem);
  for (QMap<int, Video*>::const_iterator it = this->mp_Acquisition->videos().begin() ; it != this->mp_Acquisition->videos().end() ; ++it)
    videosRoot->addChild(this->createVideoItem(it.value()->label, it.key(), it.value()->error));
  // Refresh the tree and these actions
  this->refresh();
  this->modelTree->clearSelection();
  // Enable the visual configuration
  this->modelConfigurationComboBox->setEnabled(true);
  this->modelConfigurationButton->setEnabled(true);
  // Update the recent colors
  this->drawRecentColors();
  
  this->m_DisplayedIds.resize(markersRoot->childCount());
  
  this->sendHiddenMarkers(); // For the virtual markers hidden by default.
  
  // Detect the visual configuration or reload the current one
  QFileInfo fi(this->mp_Acquisition->fileName());
  int configIndex = this->modelConfigurationComboBox->findText(fi.baseName());
  if (configIndex != -1)
    this->selectConfiguration(configIndex);
  else
  {
    fi.setFile(fi.canonicalPath() + "/" + fi.completeBaseName() + ".mvc");
    if (fi.exists())
      this->loadConfiguration(fi.canonicalFilePath());
    else if (this->m_CurrentConfigurationIndex != -1)
      this->loadConfiguration(this->m_ConfigurationItems[this->m_CurrentConfigurationIndex].filename); // Reload the current configuration
  }
};

void ModelDockWidget::reset()
{
  this->mp_NewSegment->setEnabled(false);
  // Clean the tree
  this->modelTree->blockSignals(true);
  this->modelTree->clearSelection();
  this->modelTree->blockSignals(false);
  for (int i = 0 ; i < this->modelTree->topLevelItemCount() ; ++i)
  {
    if (i == ModelOutputsItem)
      continue;
    QTreeWidgetItem* item = this->modelTree->topLevelItem(i);
    while (item->childCount() > 0)
      delete item->takeChild(0);
    item->setExpanded(false);
  }
  // Special case: model outputs
  QTreeWidgetItem* item = this->modelTree->topLevelItem(ModelOutputsItem);
  for (int i = 0 ; i < item->childCount() ; ++i)
  {
    QTreeWidgetItem* subitem = item->child(i);
    while (subitem->childCount() > 0)
      delete subitem->takeChild(0);
    subitem->setExpanded(false);
  }
  item->setExpanded(false);
  // Disable the visual configuration
  this->modelConfigurationComboBox->setEnabled(false);
  this->modelConfigurationButton->setEnabled(false);
  // Force the properties to the empty page
  this->propertiesStack->setCurrentWidget(this->emptyPage);
  // Refresh the tree and its actions
  this->refresh();
};

void ModelDockWidget::visualConfigurations(QStringList& names, QStringList& filenames)
{
  for (int i = 0 ; i < this->m_ConfigurationItems.count() ; ++i)
  {
    names << this->m_ConfigurationItems[i].name;
    filenames << this->m_ConfigurationItems[i].filename;
  }
};

void ModelDockWidget::setVisualConfigurations(const QStringList& names, const QStringList& filenames)
{
  this->modelConfigurationComboBox->blockSignals(true);
  for (int i = 0 ; i < names.count() ; ++i)
  {
    ConfigurationItem config;
    config.name = names[i];
    config.filename = filenames[i];
    config.isNew = false;
    config.isModified = false;
    this->m_ConfigurationItems.push_back(config);
    
    this->modelConfigurationComboBox->insertItem(i,names[i]);
  }
  this->modelConfigurationComboBox->setCurrentIndex(-1);
  this->modelConfigurationComboBox->blockSignals(false);
};

QList<int> ModelDockWidget::selectedMarkers() const
{
  QList<int> ids;
  QTreeWidgetItem* markersRoot = this->modelTree->topLevelItem(MarkersItem);
  for (int i = 0 ; i < markersRoot->childCount() ; ++i)
  {
    QTreeWidgetItem* item = markersRoot->child(i);
    if (item->isSelected())
      ids << item->data(0, PointId).toInt();
  }
  QTreeWidgetItem* virtualMarkersRoot = this->modelTree->topLevelItem(VirtualMarkersItem);
  for (int i = 0 ; i < virtualMarkersRoot->childCount() ; ++i)
  {
    QTreeWidgetItem* item = virtualMarkersRoot->child(i);
    if (item->isSelected())
      ids << item->data(0, PointId).toInt();
  }
  return ids;
};

QList<int> ModelDockWidget::tailedMarkers() const
{
  QList<int> ids;
  QTreeWidgetItem* markersRoot = this->modelTree->topLevelItem(MarkersItem);
  for (int i = 0 ; i < markersRoot->childCount() ; ++i)
  {
    QTreeWidgetItem* item = markersRoot->child(i);
    if (item->checkState(TrajectoryHeader) == Qt::Checked)
      ids << item->data(0, PointId).toInt();
  }
  QTreeWidgetItem* virtualMarkersRoot = this->modelTree->topLevelItem(VirtualMarkersItem);
  for (int i = 0 ; i < virtualMarkersRoot->childCount() ; ++i)
  {
    QTreeWidgetItem* item = virtualMarkersRoot->child(i);
    if (item->checkState(TrajectoryHeader) == Qt::Checked)
      ids << item->data(0, PointId).toInt();
  }
  return ids;
};

QList<int> ModelDockWidget::selectedVideos() const
{
  QList<int> ids;
  QTreeWidgetItem* videosRoot = this->modelTree->topLevelItem(VideosItem);
  for (int i = 0 ; i < videosRoot->childCount() ; ++i)
  {
    QTreeWidgetItem* item = videosRoot->child(i);
    if (item->isSelected())
      ids << item->data(0, VideoId).toInt();
  }
  return ids;
};

bool ModelDockWidget::isOkToContinue()
{
  int idx = this->m_CurrentConfigurationIndex;//this->modelConfigurationComboBox->currentIndex();
  if ((idx != -1) && (this->m_ConfigurationItems[idx].isModified))
  {
    QMessageBox messageBox(this->parentWidget());
    messageBox.setIcon(QMessageBox::Information);
    messageBox.setText(tr("<nobr>The visual configuration has been modified.</nobr>"));
    if (this->m_ConfigurationItems[idx].isNew)
      messageBox.setInformativeText(tr("Do you want to save your changes?\n\nThis configuration is a new one and will be deleted if you do not save it."));
    else
      messageBox.setInformativeText(tr("Do you want to save your changes?"));
#ifdef Q_OS_MAC
    messageBox.setWindowFlags(Qt::Sheet);
    messageBox.setWindowModality(Qt::WindowModal);
    messageBox.setStandardButtons(QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
    messageBox.setDefaultButton(QMessageBox::Save);
#else
    messageBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel);
    messageBox.setDefaultButton(QMessageBox::Yes);
#endif
    messageBox.setEscapeButton(QMessageBox::Cancel);
    int res = messageBox.exec();
    if ((res == QMessageBox::Save) ||(res == QMessageBox::Yes))
      return this->saveConfiguration(idx);
    else if ((res == QMessageBox::Discard) ||(res == QMessageBox::No))
    {
      if (this->m_ConfigurationItems[idx].isNew)
      {
        this->removeConfiguration(idx);
        this->m_CurrentConfigurationIndex = -1;
      }
      else
        this->setConfigurationModified(idx, false);
    }
    else if (res == QMessageBox::Cancel)
      return false;
  }
  return true;
};

void ModelDockWidget::selectConfiguration(int idx)
{
  this->modelConfigurationComboBox->blockSignals(true);
  this->modelConfigurationComboBox->setCurrentIndex(this->m_CurrentConfigurationIndex);
  this->modelConfigurationComboBox->blockSignals(false);
  if (this->isOkToContinue())
    this->loadConfiguration(this->m_ConfigurationItems[idx].filename);
  this->modelTree->setFocus();
};

void ModelDockWidget::setConfigurationModified(bool cleaned)
{
  int index = this->modelConfigurationComboBox->currentIndex();
  if (index != -1)
  {
    if (cleaned)
    {
      this->mp_SaveConfiguration->setEnabled(false);
      this->m_ConfigurationItems[index].isModified = false;
      this->modelConfigurationComboBox->setItemText(index, this->m_ConfigurationItems[index].name);
    }
    else
    {
      this->mp_SaveConfiguration->setEnabled(true);
      this->m_ConfigurationItems[index].isModified = true;
      this->modelConfigurationComboBox->setItemText(index, "*" + this->m_ConfigurationItems[index].name);
    }
  }
};

void ModelDockWidget::loadConfiguration(const QString& filename)
{
  QMessageBox messageBox(QMessageBox::Warning, 
                         "",
                         "Cannot read configuration",
                         QMessageBox::Ok,
                         this->parentWidget());
  messageBox.setDefaultButton(QMessageBox::Ok);
#ifdef Q_OS_MAC
  messageBox.setWindowModality(Qt::WindowModal);
  messageBox.setWindowFlags(Qt::Sheet);
#endif
  
  LOG_INFO("Loading model configuration: " + QFileInfo(filename).fileName());

  QFile file(filename);
  if (!file.open(QFile::ReadOnly | QFile::Text))
  {
    LOG_CRITICAL("Error when reading the file: " + filename + ": " + file.errorString());
    messageBox.setText(messageBox.text());
    messageBox.setInformativeText("<nobr>" + filename + ": " + file.errorString() + "</nobr>\n\nThis configuration is removed from the list");
    messageBox.exec();
    for (int i = 0 ; i < this->m_ConfigurationItems.count() ; ++i)
    {
      if (this->m_ConfigurationItems[i].filename.compare(filename) == 0)
      {
        this->removeConfiguration(i);
        break;
      }
    }
    return;
  }
  
  QString configName;
  QList<int> ids;
  QList<double> radii;
  QList<QColor> colors;
  QList<bool> markersVisibility;
  QList<bool> markersTrajectoryVisibility;
  QList<Segment> segments;
  QXmlStreamReader xmlReader(&file);
  bool isMokkaModelConfig = false;
  if (xmlReader.readNextStartElement())
  {
    // Mokka model configuration
    if ((xmlReader.name() == "MokkaModelVisualConfiguration") && (xmlReader.attributes().value("version") == "1.0"))
    {
      isMokkaModelConfig = true;
      configName = xmlReader.attributes().value("name").toString();
      while (xmlReader.readNextStartElement())
      {
        if (xmlReader.name() == "MarkersList")
        {
          while (xmlReader.readNextStartElement())
          {
            if (xmlReader.name() == "Marker")
            {
              QXmlStreamAttributes att = xmlReader.attributes();
              int id = this->mp_Acquisition->findMarkerIdFromLabel(att.value("label").toString());
              if (id != -1)
              {
                ids << id;
                radii << att.value("radius").toString().toDouble();
                colors << QColor((int)(att.value("colorR").toString().toDouble() * 255),
                                 (int)(att.value("colorG").toString().toDouble() * 255),
                                 (int)(att.value("colorB").toString().toDouble() * 255));
                QString str = att.value("visible").toString();
                markersVisibility << (str.isEmpty() ? true : (str.compare("1") == 0));
                markersTrajectoryVisibility << (att.value("trajectory").toString().compare("1") == 0);
              }
            }
            else
              xmlReader.skipCurrentElement();
            xmlReader.readNext();
          }
        }
        else if (xmlReader.name() == "SegmentsList")
        {
          while (xmlReader.readNextStartElement())
          {
            if (xmlReader.name() == "Segment")
            {
              QXmlStreamAttributes att = xmlReader.attributes();
              QString segLabel = att.value("label").toString();
              QString segDesc = att.value("description").toString();
              QString str = att.value("visible").toString();
              bool segVisible = str.isEmpty() ? true : (str.compare("1") == 0);
              bool segSurfaceVisible = att.value("surface").toString().compare("1") == 0;
              QColor segColor = QColor((int)(att.value("colorR").toString().toDouble() * 255),
                                       (int)(att.value("colorG").toString().toDouble() * 255),
                                       (int)(att.value("colorB").toString().toDouble() * 255));
              QVector<int> markerIds;
              QVector<Pair> links;
              QVector<Triad> faces;
              while (xmlReader.readNextStartElement())
              {
                if (xmlReader.name() == "Point")
                {
                  int idPt = this->mp_Acquisition->findMarkerIdFromLabel(xmlReader.attributes().value("label").toString());
                  if (idPt != -1)
                    markerIds.push_back(idPt);
                }
                else if (xmlReader.name() == "Link")
                {
                  QXmlStreamAttributes attLink = xmlReader.attributes();
                  int idPt1 = this->mp_Acquisition->findMarkerIdFromLabel(attLink.value("pt1").toString());
                  int idPt2 = this->mp_Acquisition->findMarkerIdFromLabel(attLink.value("pt2").toString());
                  if ((idPt1 != -1) && (idPt2 != -1))
                    links << Pair(idPt1, idPt2);
                }
                else if (xmlReader.name() == "Face")
                {
                  QXmlStreamAttributes attLink = xmlReader.attributes();
                  int idPt1 = this->mp_Acquisition->findMarkerIdFromLabel(attLink.value("pt1").toString());
                  int idPt2 = this->mp_Acquisition->findMarkerIdFromLabel(attLink.value("pt2").toString());
                  int idPt3 = this->mp_Acquisition->findMarkerIdFromLabel(attLink.value("pt3").toString());
                  if ((idPt1 != -1) && (idPt2 != -1) && (idPt3 != -1))
                    faces << Triad(idPt1, idPt2, idPt3);
                }
                else
                  xmlReader.skipCurrentElement();
                xmlReader.readNext();
              }
              segments << Segment(segLabel, segDesc, segColor, markerIds, links, faces, segVisible, segSurfaceVisible);
            }
            else
              xmlReader.skipCurrentElement();
            xmlReader.readNext();
          }
        }
        else
          xmlReader.skipCurrentElement();
      }
    }
    // Vicon model configuration
    else if ((xmlReader.name() == "KinematicModel"))
    {
      configName = xmlReader.attributes().value("MODEL").toString();
      if (configName.isEmpty())
      {
        LOG_WARNING("No name for the configuration. It will use its filename as name.");
        configName = QFileInfo(filename).baseName();
      }
      while (xmlReader.readNextStartElement())
      {
        if (xmlReader.name() == "Skeleton")
        {
          this->appendSegments(xmlReader, segments);
        }
        else if (xmlReader.name() == "MarkerSet")
        {
          QStringList segmentsLabel;
          while (xmlReader.readNextStartElement())
          {
            if (xmlReader.name() == "Markers")
            {
              while (xmlReader.readNextStartElement())
              {
                if (xmlReader.name() == "Marker")
                {
                  QXmlStreamAttributes att = xmlReader.attributes();
                  int id = this->mp_Acquisition->findMarkerIdFromLabel(att.value("NAME").toString());
                  if (id != -1)
                  {
                    ids << id;
                    radii << att.value("RADIUS").toString().toDouble();
                    QStringList rgb = att.value("RGB").toString().split(' ');
                    if (rgb.size() < 3)
                    {
                      btkErrorMacro("The RGB attribute for the marker element doesn't contain 3 values and is replaced by the default color.");
                      colors << this->mp_Acquisition->defaultMarkerColor();
                    }
                    else
                      colors << QColor(rgb[0].toInt(), rgb[1].toInt(), rgb[2].toInt());
                    markersVisibility << true;
                    markersTrajectoryVisibility << false;
                    segmentsLabel << att.value("SEGMENT").toString();
                  }
                  xmlReader.skipCurrentElement();
                }
                else
                  xmlReader.skipCurrentElement();
                xmlReader.readNext();
              }
            }
            else if (xmlReader.name() == "Sticks")
            {
              while (xmlReader.readNextStartElement())
              {
                if (xmlReader.name() == "Stick")
                {
                  QXmlStreamAttributes att = xmlReader.attributes();
                  int idPt1 = this->mp_Acquisition->findMarkerIdFromLabel(att.value("MARKER1").toString());
                  int idPt2 = this->mp_Acquisition->findMarkerIdFromLabel(att.value("MARKER2").toString());
                  if ((idPt1 != -1) && (idPt2 != -1))
                  {
                    for (int i = 0 ; i < ids.size() ; ++i)
                    {
                      if (ids[i] == idPt2)
                      {
                        for (int j = 0 ; j < segments.size() ; ++j)
                        {
                          if (segments[j].label.compare(segmentsLabel[i]) == 0)
                          {
                            if (segments[j].markerIds.indexOf(idPt1) == -1)
                              segments[j].markerIds.push_back(idPt1);
                            if (segments[j].markerIds.indexOf(idPt2) == -1)
                              segments[j].markerIds.push_back(idPt2);
                            segments[j].links.push_back(Pair(idPt1, idPt2));
                            break;
                          }
                        }
                        break;
                      }
                    }
                  }
                }
                else
                  xmlReader.skipCurrentElement();
                xmlReader.readNext();
              }
            }
            else
              xmlReader.skipCurrentElement();
            xmlReader.readNext();
          }
        }
        else
          xmlReader.skipCurrentElement();
      }
      // Synchronize the segment's infos and the corresponding mesh.
      for (int i = 0 ; i < segments.size() ; ++i)
        segments[i].mesh->SetDefinition(segments[i].markerIds.toStdVector(), segments[i].links.toStdVector(), segments[i].faces.toStdVector());
    }
    else
      xmlReader.raiseError(QObject::tr("The file is not a Mokka Model Visual Configuration file."));
  }
  file.close();
  if (xmlReader.hasError())
  {
    QString errMsg = "Failed to parse file: " + filename + "\n" + xmlReader.errorString();
    LOG_CRITICAL(errMsg);
    messageBox.setText(errMsg);
    messageBox.exec();
    return;
  }
  else if (file.error() != QFile::NoError)
  {
    QString errMsg = "Cannot read file: " + filename + "\n" + file.errorString();
    LOG_CRITICAL(errMsg);
    messageBox.setText(errMsg);
    messageBox.exec();
    return;
  }
  
  this->modelConfigurationComboBox->blockSignals(true);
  int id = -1;
  for (int i = 0 ; i < this->m_ConfigurationItems.count() ; ++i)
  {
    if (this->m_ConfigurationItems[i].name.compare(configName) == 0)
    {
      id = i;
      break;
    }
  }
  if (id == -1)
  {
    ConfigurationItem config;
    config.name = configName;
    config.isModified = false;
    config.filename = filename;
    config.isNew = false;
    this->m_ConfigurationItems.push_back(config);
    this->modelConfigurationComboBox->addItem(configName);
    id = this->m_ConfigurationItems.count()-1;
  }
  this->m_ConfigurationItems[id].isNew = (isMokkaModelConfig ? false : true); // Force the flag to not save over other file format than MVC.
  this->setCurrentConfiguration(id);
  this->mp_SaveConfiguration->setEnabled(false);
  this->mp_RemoveConfiguration->setEnabled(true);
  this->mp_DeselectConfiguration->setEnabled(true);
  this->modelConfigurationComboBox->blockSignals(false);
  
  this->mp_Acquisition->resetMarkersConfiguration(ids, markersVisibility, markersTrajectoryVisibility, radii, colors);
  this->mp_Model->setSegments(segments);
};

void ModelDockWidget::appendSegments(QXmlStreamReader& xmlReader, QList<Segment>& segments) const
{
  while (xmlReader.readNextStartElement())
  {
    if (xmlReader.name() == "Segment")
    {
      QXmlStreamAttributes att = xmlReader.attributes();
      QString segLabel = att.value("NAME").toString();
      QStringList rgb = att.value("RGB").toString().split(' ');
      QColor segColor;
      if (rgb.size() < 3)
      {
        btkErrorMacro("The RGB attribute for the segment element doesn't contain 3 values and is replaced by the default color.");
        segColor = this->mp_Model->defaultSegmentColor();
      }
      else
        segColor = QColor(rgb[0].toInt(), rgb[1].toInt(), rgb[2].toInt());
      segments << Segment(segLabel, "", segColor, QVector<int>(), QVector<Pair>(), QVector<Triad>());
      
      this->appendSegments(xmlReader, segments);
    }
    else
      xmlReader.skipCurrentElement();
    xmlReader.readNext();
  }
};

void ModelDockWidget::newConfiguration()
{
  if (!this->isOkToContinue())
    return;
  NewModelDialog nmd(&(this->m_ConfigurationItems), this->parentWidget());
  if (this->mp_Acquisition)
  {
    QFileInfo fileInfo(this->mp_Acquisition->fileName());
    QString name = fileInfo.baseName();
    nmd.setConfigurationName(name);
  }
  this->modelConfigurationComboBox->blockSignals(true);
  if (nmd.exec())
  {
    int modelIndex;
    QString name = nmd.configurationName(&modelIndex);
    if (modelIndex == -1) // new
    {
      ConfigurationItem config;
      config.name = name;
      config.isNew = true;
      config.filename = "";
      config.isModified = true;
      this->m_ConfigurationItems.push_back(config);
      modelIndex = this->m_ConfigurationItems.count()-1;
      this->modelConfigurationComboBox->addItem("*" + name);
    }
    else
    {
      ConfigurationItem* config = &(this->m_ConfigurationItems[modelIndex]);
      config->isModified = true;
      this->modelConfigurationComboBox->setItemText(modelIndex, "*" + name);
    }
    this->setCurrentConfiguration(modelIndex);
    
    this->mp_SaveConfiguration->setEnabled(true);
    this->mp_RemoveConfiguration->setEnabled(true);
    this->mp_DeselectConfiguration->setEnabled(true);
  }
  this->modelConfigurationComboBox->blockSignals(false);
};

void ModelDockWidget::loadConfiguration()
{
  if (!this->isOkToContinue())
    return;
  
  QFileInfo fileInfo(this->mp_Acquisition->fileName());
  QString filename = QFileDialog::getOpenFileName(this,
    tr("Open Model Visual Configuration"),
    fileInfo.path(),
    tr("Model Configuration Files (*.mvc *.vst *.vsk);;"
       "Mokka Model Configuration Files (*.mvc);;"
       "Vicon Model Configuration Files (*.vsk *.vst)"));
  if (!filename.isEmpty())
    this->loadConfiguration(filename);
};

void ModelDockWidget::saveConfiguration()
{
  this->saveConfiguration(this->modelConfigurationComboBox->currentIndex());
};

void ModelDockWidget::removeConfiguration()
{
  if (!this->isOkToContinue())
    return;
  this->removeConfiguration(this->modelConfigurationComboBox->currentIndex());
};

void ModelDockWidget::deselectConfiguration()
{
  if (!this->isOkToContinue())
    return;
  this->modelConfigurationComboBox->blockSignals(true);
  this->modelConfigurationComboBox->setCurrentIndex(-1);
  this->m_CurrentConfigurationIndex = -1;
  this->mp_SaveConfiguration->setEnabled(false);
  this->mp_RemoveConfiguration->setEnabled(false);
  this->mp_DeselectConfiguration->setEnabled(false);
  this->modelConfigurationComboBox->blockSignals(false);
};

void ModelDockWidget::clearConfigurations()
{
  this->m_ConfigurationItems.clear();
  this->modelConfigurationComboBox->blockSignals(true);
  this->modelConfigurationComboBox->setCurrentIndex(-1);
  this->m_CurrentConfigurationIndex = -1;
  this->modelConfigurationComboBox->clear();
  this->mp_SaveConfiguration->setEnabled(false);
  this->mp_RemoveConfiguration->setEnabled(false);
  this->mp_DeselectConfiguration->setEnabled(false);
  this->modelConfigurationComboBox->blockSignals(false);
};

void ModelDockWidget::hideSelectedSegments()
{
  this->modelTree->blockSignals(true);
  QList<QTreeWidgetItem*> items = this->modelTree->selectedItems();
  for (QList<QTreeWidgetItem*>::const_iterator it = items.begin() ; it != items.end() ; ++it)
    (*it)->setCheckState(VisibleHeader, Qt::Unchecked);
  this->modelTree->blockSignals(false);
  this->sendHiddenSegments();
};

void ModelDockWidget::unhideSelectedSegments()
{
  this->modelTree->blockSignals(true);
  QList<QTreeWidgetItem*> items = this->modelTree->selectedItems();
  for (QList<QTreeWidgetItem*>::const_iterator it = items.begin() ; it != items.end() ; ++it)
    (*it)->setCheckState(VisibleHeader, Qt::Checked);
  this->modelTree->blockSignals(false);
  this->sendHiddenSegments();
};

void ModelDockWidget::setSegments(const QList<int>& ids, const QList<Segment*>& segments)
{
  QTreeWidgetItem* segmentsRoot = this->modelTree->topLevelItem(SegmentsItem);
  while (segmentsRoot->childCount() > 0)
    delete segmentsRoot->takeChild(0);
  emit segmentsCleared();
  this->insertSegments(ids, segments);
  this->modelTree->clearSelection();
};

void ModelDockWidget::setSegmentLabel(int id, const QString& label)
{
  QTreeWidgetItem* segmentsRoot = this->modelTree->topLevelItem(SegmentsItem);
  for (int i = 0 ; i < segmentsRoot->childCount() ; ++i)
  {
    if (segmentsRoot->child(i)->data(0, SegmentId).toInt() == id)
    {
      segmentsRoot->child(i)->setText(0, label);
      break;
    }
  }
  this->displayProperties();
};

void ModelDockWidget::setSegmentsColor(const QVector<int>& ids, const QVector<QColor>& colors)
{
  // Upate the tree
  QTreeWidgetItem* segmentsRoot = this->modelTree->topLevelItem(SegmentsItem);
  for (int i = 0 ; i < ids.count() ; ++i)
  {
    for (int j = 0 ; j < segmentsRoot->childCount() ; ++j)
    {
      if (segmentsRoot->child(j)->data(0, SegmentId).toInt() == ids[i])
      {
        segmentsRoot->child(j)->setIcon(0, this->createSegmentIcon(colors[i]));
        break;
      }
    }
  }
  this->displayProperties();
};

void ModelDockWidget::setSegmentsDescription(const QVector<int>& ids, const QVector<QString>& descs)
{
  Q_UNUSED(ids);
  Q_UNUSED(descs);
  this->displayProperties();
};

void ModelDockWidget::setSegmentsVisibility(const QVector<int>& ids, const QVector<bool>& visibles)
{
  this->modelTree->blockSignals(true);
  QTreeWidgetItem* segmentsRoot = this->modelTree->topLevelItem(SegmentsItem);
  for (int i = 0 ; i < ids.size() ; ++i)
  {
    for (int j = 0 ; j < segmentsRoot->childCount() ; ++j)
    {
      QTreeWidgetItem* item = segmentsRoot->child(j);
      if (item->data(0, SegmentId).toInt() == ids[i])
      {
        item->setCheckState(VisibleHeader, visibles[i] ? Qt::Checked : Qt::Unchecked);
        break;
      }
    }
  }
  this->modelTree->blockSignals(false);
  this->displayProperties();
};

void ModelDockWidget::setSegmentsSurfaceVisibility(const QVector<int>& ids, const QVector<bool>& visibles)
{
  Q_UNUSED(ids);
  Q_UNUSED(visibles);
  this->displayProperties();
};

void ModelDockWidget::removeSegments(const QList<int>& ids, const QList<Segment*>& segments)
{
  Q_UNUSED(segments);
  this->modelTree->blockSignals(true);
  bool itemUnselected = false;
  QTreeWidgetItem* segmentsRoot = this->modelTree->topLevelItem(SegmentsItem);
  for (int i = 0 ; i < ids.count() ; ++i)
  {
    QTreeWidgetItem* item = segmentsRoot->child(ids[i]);
    if (item->isSelected())
    {
      this->modelTree->setCurrentItem(item, 0, QItemSelectionModel::Deselect);
      this->modelTree->setCurrentItem(item, 1, QItemSelectionModel::Deselect);
      this->modelTree->setCurrentItem(item, 2, QItemSelectionModel::Deselect);
      itemUnselected = true;
    }
    item->setHidden(true);
  }
  this->modelTree->blockSignals(false);
  this->sendHiddenSegments();
  this->refresh();
  if (itemUnselected)
  {
    this->segmentLabelEdit->blockSignals(true);
    this->segmentDescEdit->blockSignals(true);
    this->displayProperties();
    this->segmentLabelEdit->blockSignals(false);
    this->segmentDescEdit->blockSignals(false);
  }
};

void ModelDockWidget::insertSegments(const QList<int>& ids, const QList<Segment*>& segments)
{
  QTreeWidgetItem* segmentsRoot = this->modelTree->topLevelItem(SegmentsItem);
  int count = segmentsRoot->childCount();
  QList<int> newIds;
  QList<Segment*> newSegments;
  for (int i = 0 ; i < ids.count() ; ++i)
  {
    if (segments[i]->isNew)
    {
      QTreeWidgetItem* item = this->createSegmentItem(segments[i]->label, ids[i], segments[i]->visible);
      segmentsRoot->addChild(item);
      this->modelTree->setCurrentItem(item); 
      segments[i]->isNew = false;
      newIds << ids[i];
      newSegments << segments[i];
    }
    segmentsRoot->child(ids[i])->setHidden(false);
  }
  if (count != segmentsRoot->childCount())
    emit newSegmentsInserted(newIds, newSegments);
  this->sendHiddenSegments();
  this->refresh();
  segmentsRoot->setExpanded(true);
  this->modelTree->scrollToItem(segmentsRoot);
};

void ModelDockWidget::editSegmentLabel()
{
  int id = this->modelTree->currentItem()->data(0, SegmentId).toInt();
  QString label = this->segmentLabelEdit->text();
  if (label.compare(this->mp_Model->segmentLabel(id)) != 0)
    emit segmentLabelChanged(id, label);
}

void ModelDockWidget::editSegmentsDescription()
{
  QList<QTreeWidgetItem*> items = this->modelTree->selectedItems();
  QVector<int> ids = QVector<int>(items.count());
  int inc = 0;
  QString desc = this->segmentDescEdit->text();
  bool descModified = false;
  for (QList<QTreeWidgetItem*>::const_iterator it = items.begin() ; it != items.end() ; ++it)
  {
    ids[inc] = (*it)->data(0, SegmentId).toInt();
    if (this->mp_Model->segmentDescription(ids[inc]).compare(desc) != 0)
      descModified = true;
    ++inc;
  }
  if (descModified)
    emit segmentsDescriptionChanged(ids, desc);
};

void ModelDockWidget::editSegmentsVisibility()
{
  this->modelTree->blockSignals(true);
  QList<QTreeWidgetItem*> items = this->modelTree->selectedItems();
  QVector<int> ids = QVector<int>(items.count());
  int inc = 0;
  bool checked = this->segmentVisibilityCheckBox->checkState() == Qt::Checked;
  bool visibilityModified = false;
  for (QList<QTreeWidgetItem*>::const_iterator it = items.begin() ; it != items.end() ; ++it)
  {
    (*it)->setCheckState(VisibleHeader, checked ? Qt::Checked : Qt::Unchecked);
    ids[inc] = (*it)->data(0, SegmentId).toInt();
    if (this->mp_Model->segmentVisible(ids[inc]) != checked)
      visibilityModified = true;
    ++inc;
  }
  this->modelTree->blockSignals(false);
  if (visibilityModified)
    emit segmentsVisibilityChanged(ids, checked);
};

void ModelDockWidget::editSegmentsSurfaceVisibility()
{
  QList<QTreeWidgetItem*> items = this->modelTree->selectedItems();
  QVector<int> ids = QVector<int>(items.count());
  int inc = 0;
  bool checked = this->segmentSurfaceVisibilityCheckBox->checkState() == Qt::Checked;
  bool visibilityModified = false;
  for (QList<QTreeWidgetItem*>::const_iterator it = items.begin() ; it != items.end() ; ++it)
  {
    ids[inc] = (*it)->data(0, SegmentId).toInt();
    if (this->mp_Model->segmentSurfaceVisible(ids[inc]) != checked)
      visibilityModified = true;
    ++inc;
  }
  if (visibilityModified)
    emit segmentsSurfaceVisibilityChanged(ids, checked);
};

void ModelDockWidget::updateDisplayedMarkers(const QVector<int>& ids)
{
  this->modelTree->blockSignals(true);
  QList<QTreeWidgetItem*> roots;
  roots << this->modelTree->topLevelItem(MarkersItem);
  roots << this->modelTree->topLevelItem(VirtualMarkersItem);
  for (QList<QTreeWidgetItem*>::iterator itR = roots.begin() ; itR != roots.end() ; ++itR)
  {
    for (int j = 0 ; j < (*itR)->childCount() ; ++j)
    {
      QTreeWidgetItem* item = (*itR)->child(j);
      int id = item->data(0,PointId).toInt();
      if ((id < ids.count()) && ids[id] && (item->checkState(VisibleHeader) == Qt::Checked))
        item->setForeground(0,displayLabelColor);
      else
        item->setForeground(0,defaultLabelColor);
    }
  }
  this->modelTree->blockSignals(false);
};

void ModelDockWidget::setTrackedMarkers(const QList<int>& ids)
{
  this->modelTree->blockSignals(true);
  QList<QTreeWidgetItem*> roots;
  roots << this->modelTree->topLevelItem(MarkersItem);
  roots << this->modelTree->topLevelItem(VirtualMarkersItem);
  for (QList<QTreeWidgetItem*>::iterator itR = roots.begin() ; itR != roots.end() ; ++itR)
  {
    for (int j = 0 ; j < (*itR)->childCount() ; ++j)
    {
      QTreeWidgetItem* item = (*itR)->child(j);
      if (ids.contains(item->data(0,PointId).toInt()))
        item->setCheckState(TrajectoryHeader, Qt::Checked);
      else
        item->setCheckState(TrajectoryHeader, Qt::Unchecked);
    }
  }
  this->modelTree->blockSignals(false);
  emit markerTrajectorySelectionChanged(ids);
}

void ModelDockWidget::selectMarkers(QList<int> ids)
{
  this->modelTree->blockSignals(true);
  this->modelTree->clearSelection();
  
  QList<QTreeWidgetItem*> roots;
  roots << this->modelTree->topLevelItem(MarkersItem);
  roots << this->modelTree->topLevelItem(VirtualMarkersItem);
  for (int i = 0 ; i < ids.size() ; ++i)
  {
    for (QList<QTreeWidgetItem*>::iterator itR = roots.begin() ; itR != roots.end() ; ++itR)
    {
      bool found = false;
      for (int j = 0 ; j < (*itR)->childCount() ; ++j)
      {
        QTreeWidgetItem* item = (*itR)->child(j);
        if (item->data(0, PointId).toInt() == ids[i])
        {
          item->setSelected(true);
          this->modelTree->scrollToItem(item);
          found = true;
          break;
        }
      }
      if (found) break;
    }
  }
  this->modelTree->blockSignals(false);
  this->displayProperties();
  this->sendSelectedMarkers();
};

void ModelDockWidget::selectAllMarkers()
{
  this->modelTree->blockSignals(true);
  this->modelTree->clearSelection();
  QTreeWidgetItem* markersRoot = this->modelTree->topLevelItem(MarkersItem);
  markersRoot->setExpanded(true);
  this->modelTree->scrollToItem(markersRoot, QAbstractItemView::PositionAtTop);
  for (int i = 0 ; i < markersRoot->childCount() ; ++i)
    markersRoot->child(i)->setSelected(true);
  this->modelTree->blockSignals(false);
  this->displayProperties();
  this->sendSelectedMarkers();
};

void ModelDockWidget::hideSelectedMarkers()
{
  this->modelTree->blockSignals(true);
  QList<QTreeWidgetItem*> items = this->modelTree->selectedItems();
  for (QList<QTreeWidgetItem*>::const_iterator it = items.begin() ; it != items.end() ; ++it)
    (*it)->setCheckState(VisibleHeader, Qt::Unchecked);
  this->modelTree->blockSignals(false);
  this->sendHiddenMarkers();
};

void ModelDockWidget::unhideSelectedMarkers()
{
  this->modelTree->blockSignals(true);
  QList<QTreeWidgetItem*> items = this->modelTree->selectedItems();
  for (QList<QTreeWidgetItem*>::const_iterator it = items.begin() ; it != items.end() ; ++it)
    (*it)->setCheckState(VisibleHeader, Qt::Checked);
  this->modelTree->blockSignals(false);
  this->sendHiddenMarkers();
};

void ModelDockWidget::trackSelectedMarkers()
{
  this->modelTree->blockSignals(true);
  QList<QTreeWidgetItem*> items = this->modelTree->selectedItems();
  for (QList<QTreeWidgetItem*>::const_iterator it = items.begin() ; it != items.end() ; ++it)
    (*it)->setCheckState(TrajectoryHeader, Qt::Checked);
  this->modelTree->blockSignals(false);
  this->sendTrackedMarkers();
};

void ModelDockWidget::untrackSelectedMarkers()
{
  this->modelTree->blockSignals(true);
  QList<QTreeWidgetItem*> items = this->modelTree->selectedItems();
  for (QList<QTreeWidgetItem*>::const_iterator it = items.begin() ; it != items.end() ; ++it)
    (*it)->setCheckState(TrajectoryHeader, Qt::Unchecked);
  this->modelTree->blockSignals(false);
  this->sendTrackedMarkers();
};

void ModelDockWidget::selectAllAnalogs()
{
  this->modelTree->blockSignals(true);
  this->modelTree->clearSelection();
  QTreeWidgetItem* analogsRoot = this->modelTree->topLevelItem(AnalogsItem);
  analogsRoot->setExpanded(true);
  this->modelTree->scrollToItem(analogsRoot, QAbstractItemView::PositionAtTop);
  for (int i = 0 ; i < analogsRoot->childCount() ; ++i)
    analogsRoot->child(i)->setSelected(true);
  this->modelTree->blockSignals(false);
  this->displayProperties();
};

void ModelDockWidget::selectAllModelOutputs()
{
  this->selectAllScalars(false);
  this->selectAllPowers(true);
  this->selectAllMoments(true);
  this->selectAllForces(true);
  this->selectAllAngles(true);
};

void ModelDockWidget::selectAllAngles(bool appending)
{
  this->modelTree->blockSignals(true);
  if (!appending)
    this->modelTree->clearSelection();
  QTreeWidgetItem* modelOutputsRoot = this->modelTree->topLevelItem(ModelOutputsItem);
  QTreeWidgetItem* anglesRoot = modelOutputsRoot->child(0);
  anglesRoot->setExpanded(true);
  modelOutputsRoot->setExpanded(true);
  this->modelTree->scrollToItem(anglesRoot, QAbstractItemView::PositionAtTop);
  for (int i = 0 ; i < anglesRoot->childCount() ; ++i)
    anglesRoot->child(i)->setSelected(true);
  this->modelTree->blockSignals(false);
  this->displayProperties();
};

void ModelDockWidget::selectAllForces(bool appending)
{
  this->modelTree->blockSignals(true);
  if (!appending)
    this->modelTree->clearSelection();
  QTreeWidgetItem* modelOutputsRoot = this->modelTree->topLevelItem(ModelOutputsItem);
  QTreeWidgetItem* forcesRoot = modelOutputsRoot->child(1);
  forcesRoot->setExpanded(true);
  modelOutputsRoot->setExpanded(true);
  this->modelTree->scrollToItem(forcesRoot, QAbstractItemView::PositionAtTop);
  for (int i = 0 ; i < forcesRoot->childCount() ; ++i)
    forcesRoot->child(i)->setSelected(true);
  this->modelTree->blockSignals(false);
  this->displayProperties();
};

void ModelDockWidget::selectAllMoments(bool appending)
{
  this->modelTree->blockSignals(true);
  if (!appending)
    this->modelTree->clearSelection();
  QTreeWidgetItem* modelOutputsRoot = this->modelTree->topLevelItem(ModelOutputsItem);
  QTreeWidgetItem* momentsRoot = modelOutputsRoot->child(2);
  momentsRoot->setExpanded(true);
  modelOutputsRoot->setExpanded(true);
  this->modelTree->scrollToItem(momentsRoot, QAbstractItemView::PositionAtTop);
  for (int i = 0 ; i < momentsRoot->childCount() ; ++i)
    momentsRoot->child(i)->setSelected(true);
  this->modelTree->blockSignals(false);
  this->displayProperties();
};

void ModelDockWidget::selectAllPowers(bool appending)
{
  this->modelTree->blockSignals(true);
  if (!appending)
    this->modelTree->clearSelection();
  QTreeWidgetItem* modelOutputsRoot = this->modelTree->topLevelItem(ModelOutputsItem);
  QTreeWidgetItem* powersRoot = modelOutputsRoot->child(3);
  powersRoot->setExpanded(true);
  modelOutputsRoot->setExpanded(true);
  this->modelTree->scrollToItem(powersRoot, QAbstractItemView::PositionAtTop);
  for (int i = 0 ; i < powersRoot->childCount() ; ++i)
    powersRoot->child(i)->setSelected(true);
  this->modelTree->blockSignals(false);
  this->displayProperties();
};

void ModelDockWidget::selectAllScalars(bool appending)
{
  this->modelTree->blockSignals(true);
  if (!appending)
    this->modelTree->clearSelection();
  QTreeWidgetItem* modelOutputsRoot = this->modelTree->topLevelItem(ModelOutputsItem);
  QTreeWidgetItem* scalarsRoot = modelOutputsRoot->child(4);
  scalarsRoot->setExpanded(true);
  modelOutputsRoot->setExpanded(true);
  this->modelTree->scrollToItem(scalarsRoot, QAbstractItemView::PositionAtTop);
  for (int i = 0 ; i < scalarsRoot->childCount() ; ++i)
    scalarsRoot->child(i)->setSelected(true);
  this->modelTree->blockSignals(false);
  this->displayProperties();
};

void ModelDockWidget::removeSelectedItems()
{
  QList<QTreeWidgetItem*> items = this->modelTree->selectedItems();
  QList<int> SegmentIds, PointIds, AnalogIds, VideoIds;
  bool platformWarningDisplayed = false;
  for (QList<QTreeWidgetItem*>::const_iterator it = items.begin() ; it != items.end() ; ++it)
  {
    QVariant sV = (*it)->data(0, SegmentId);
    QVariant pV = (*it)->data(0, PointId);
    QVariant aV = (*it)->data(0, AnalogId);
    QVariant vV = (*it)->data(0, VideoId);
    if (sV.isValid())
      SegmentIds.push_back(sV.toInt());
    if (pV.isValid())
      PointIds.push_back(pV.toInt());
    else if (aV.isValid())
      AnalogIds.push_back(aV.toInt());
    else if ((*it)->data(0, ForcePlateId).isValid() && !platformWarningDisplayed)
    {
      LOG_WARNING("You cannot delete force platform component, nor force platform itself. These data are computed from the analog channels and some metadata.");
      platformWarningDisplayed = true;
    }
    else if (vV.isValid())
      VideoIds.push_back(vV.toInt());
  }
  if (!SegmentIds.isEmpty())
    emit segmentsRemoved(SegmentIds);
  if (!PointIds.isEmpty())
    emit pointsRemoved(PointIds);
  if (!AnalogIds.isEmpty())
    emit analogsRemoved(AnalogIds);
  if (!VideoIds.isEmpty())
    emit videosRemoved(VideoIds);
  this->modelTree->clearSelection();
};

void ModelDockWidget::toggleProperties()
{
  // The use of a static variable is ok as the properties are expanded during the UI init.
  static int propertiesExpandedSize;
  if (this->propertiesGroup->isVisible())
    propertiesExpandedSize = this->splitter->sizes()[1];
  
  if (!this->propertiesGroup->isVisible())
  {
    this->propertiesGroup->setVisible(true);
    this->propertiesButton->setIcon(*this->mp_DownArrowIcon);
    // Force to have the same size than the previous expanding.
    QList<int> sizes = this->splitter->sizes();
    sizes[0] = sizes[0] + sizes[1] - propertiesExpandedSize;
    sizes[1] = propertiesExpandedSize;
    this->splitter->setSizes(sizes);
  }
  else
  {
    this->propertiesGroup->setVisible(false);
    this->propertiesButton->setIcon(*this->mp_RightArrowIcon);
    // Force the properties button to move down
    QList<int> sizes = this->splitter->sizes();
    sizes[0] = sizes[0] + sizes[1] - 16;
    sizes[1] = 16;
    this->splitter->setSizes(sizes);
  }
};

void ModelDockWidget::displayProperties()
{
  this->mp_NewSegment->setEnabled(false);
  this->mp_HideSelectedSegments->setEnabled(false);
  this->mp_UnhideSelectedSegments->setEnabled(false);
  this->mp_HideSelectedMarkers->setEnabled(false);
  this->mp_UnhideSelectedMarkers->setEnabled(false);
  this->mp_TrackSelectedMarkers->setEnabled(false);
  this->mp_UntrackSelectedMarkers->setEnabled(false);
  this->mp_DeleteSelectedMarkers->setEnabled(false);
  this->mp_RemoveSelectedItems->setEnabled(false);
  bool multipleSelection = false;
  QList<QTreeWidgetItem*> items = this->modelTree->selectedItems();
  if (items.size() == 0)
  {
    this->propertiesStack->setCurrentWidget(this->emptyPage);
    return;
  }
  else if (items.size() != 1)
  {
    // Check if the selection contains mixed types (markers, others points, etc.)
    for (QList<QTreeWidgetItem*>::const_iterator it = items.begin() ; it != items.end() ; ++it)
    {
      if ((*it)->type() != items.first()->type())
      {
        this->propertiesStack->setCurrentWidget(this->emptyPage);
        return;
      }
    }
    multipleSelection = true;
  }
  this->mp_RemoveSelectedItems->setEnabled(true);
  switch(items.first()->type())
  {
  case SegmentType:
    {
    this->segmentVisibilityCheckBox->blockSignals(true);
    this->segmentSurfaceVisibilityCheckBox->blockSignals(true);
    this->mp_HideSelectedSegments->setEnabled(true);
    this->mp_UnhideSelectedSegments->setEnabled(true);
    int id = items.first()->data(0,SegmentId).toInt();
    this->segmentLabelEdit->setText(multipleSelection ? "" : this->mp_Model->segmentLabel(id));
    this->segmentLabelEdit->setEnabled(!multipleSelection);
    this->segmentDescEdit->setText(multipleSelection ? "" : this->mp_Model->segmentDescription(id));
    this->segmentEditLinksButton->setEnabled(!multipleSelection);
    this->mp_SetMarkerColorInHistory1->setEnabled(true);
    this->mp_SetMarkerColorInHistory2->setEnabled(true);
    this->mp_SetMarkerColorInHistory3->setEnabled(true);
    this->mp_SetMarkerColorInHistory4->setEnabled(true);
    this->mp_SetMarkerColorInHistory5->setEnabled(true);
    if (!multipleSelection)
    {
      QColor c = this->mp_Model->segmentColor(id);
      this->markerColorButton->setProperty("markerColor", c);
      this->markerColorButton->setIcon(this->createMarkerIcon(c, true));
      this->segmentVisibilityCheckBox->setCheckState(this->mp_Model->segmentVisible(id) ? Qt::Checked : Qt::Unchecked);
      this->segmentSurfaceVisibilityCheckBox->setCheckState(this->mp_Model->segmentSurfaceVisible(id) ? Qt::Checked : Qt::Unchecked);
    }
    else
    {
      bool uniqueColor = true;
      QColor c = this->mp_Model->segmentColor(id);
      bool visibleChecked = this->mp_Model->segmentVisible(id);
      Qt::CheckState visible = visibleChecked ? Qt::Checked : Qt::Unchecked;
      bool surfaceVisibleChecked = this->mp_Model->segmentSurfaceVisible(id);
      Qt::CheckState surfaceVisible = surfaceVisibleChecked ? Qt::Checked : Qt::Unchecked;
      QList<QTreeWidgetItem*>::const_iterator it = items.begin();
      ++it;
      while (it != items.end())
      {
        id = (*it)->data(0,SegmentId).toInt();
        if (this->mp_Model->segmentColor(id) != c)
          uniqueColor = false;
        if (this->mp_Model->segmentVisible(id) != visibleChecked)
          visible = Qt::PartiallyChecked;
        if (this->mp_Model->segmentSurfaceVisible(id) != surfaceVisibleChecked)
          surfaceVisible = Qt::PartiallyChecked;
        ++it;
      }
      this->markerColorButton->setProperty("markerColor", uniqueColor ? c : QColor::Invalid);
      this->markerColorButton->setIcon(this->createMarkerIcon(uniqueColor ? c : Qt::transparent, true, uniqueColor ? true : false));
      this->mp_SetMarkerColorInHistory1->setEnabled(uniqueColor ? true : false);
      this->mp_SetMarkerColorInHistory2->setEnabled(uniqueColor ? true : false);
      this->mp_SetMarkerColorInHistory3->setEnabled(uniqueColor ? true : false);
      this->mp_SetMarkerColorInHistory4->setEnabled(uniqueColor ? true : false);
      this->mp_SetMarkerColorInHistory5->setEnabled(uniqueColor ? true : false);
      this->segmentVisibilityCheckBox->setCheckState(visible);
      this->segmentSurfaceVisibilityCheckBox->setCheckState(surfaceVisible);
    }
    this->segmentVisibilityCheckBox->blockSignals(false);
    this->segmentSurfaceVisibilityCheckBox->blockSignals(false);
    static_cast<QGridLayout*>(this->segmentPage->layout())->addWidget(this->frame,1,1);
    this->propertiesStack->setCurrentWidget(this->segmentPage);
    break;
    }
  case MarkerType:
    {
    this->markerRadiusSpinBox->blockSignals(true);
    this->markerVisibilityCheckBox->blockSignals(true);
    this->markerTrajectoryVisibilityCheckBox->blockSignals(true);
    this->mp_HideSelectedMarkers->setEnabled(true);
    this->mp_TrackSelectedMarkers->setEnabled(true);
    this->mp_UntrackSelectedMarkers->setEnabled(true);
    this->mp_UnhideSelectedMarkers->setEnabled(true);
    this->mp_DeleteSelectedMarkers->setEnabled(true);
    int id = items.first()->data(0,PointId).toInt();
    this->markerLabelEdit->setText(multipleSelection ? "" : this->mp_Acquisition->pointLabel(id));
    this->markerLabelEdit->setEnabled(!multipleSelection);
    this->markerDescEdit->setText(multipleSelection ? "" : this->mp_Acquisition->pointDescription(id));
    this->mp_SetMarkerColorInHistory1->setEnabled(true);
    this->mp_SetMarkerColorInHistory2->setEnabled(true);
    this->mp_SetMarkerColorInHistory3->setEnabled(true);
    this->mp_SetMarkerColorInHistory4->setEnabled(true);
    this->mp_SetMarkerColorInHistory5->setEnabled(true);
    if (!multipleSelection)
    {
      double r = this->mp_Acquisition->markerRadius(id);
      this->markerRadiusSpinBox->setValue(r);
      // this->markerRadiusSlider->setValue(static_cast<int>(r));
      this->updateMarkerRadius(r); // slider
      QColor c = this->mp_Acquisition->markerColor(id);
      this->markerColorButton->setProperty("markerColor", c);
      this->markerColorButton->setIcon(this->createMarkerIcon(c, true));
      this->markerVisibilityCheckBox->setCheckState(this->mp_Acquisition->markerVisible(id) ? Qt::Checked : Qt::Unchecked);
      this->markerTrajectoryVisibilityCheckBox->setCheckState(this->mp_Acquisition->markerTrajectoryVisible(id) ? Qt::Checked : Qt::Unchecked);
    }
    else
    {
      this->mp_NewSegment->setEnabled(true);
      bool uniqueRadius = true;
      bool uniqueColor = true;
      double r = this->mp_Acquisition->markerRadius(id);
      QColor c = this->mp_Acquisition->markerColor(id);
      bool visibleChecked = this->mp_Acquisition->markerVisible(id);
      Qt::CheckState visible = visibleChecked ? Qt::Checked : Qt::Unchecked;
      bool trajectoryVisibleChecked = this->mp_Acquisition->markerTrajectoryVisible(id);
      Qt::CheckState trajectoryVisible = trajectoryVisibleChecked ? Qt::Checked : Qt::Unchecked;
      QList<QTreeWidgetItem*>::const_iterator it = items.begin();
      ++it;
      while (it != items.end())
      {
        id = (*it)->data(0,PointId).toInt();
        if (this->mp_Acquisition->markerRadius(id) != r)
          uniqueRadius = false;
        if (this->mp_Acquisition->markerColor(id) != c)
          uniqueColor = false;
        if (this->mp_Acquisition->markerVisible(id) != visibleChecked)
          visible = Qt::PartiallyChecked;
        if (this->mp_Acquisition->markerTrajectoryVisible(id) != trajectoryVisibleChecked)
          trajectoryVisible = Qt::PartiallyChecked;
        ++it;
      }
      if (uniqueRadius)
      {
        this->markerRadiusSpinBox->setValue(r);
        this->markerRadiusSpinBox->setSuffix(" mm");
        //this->markerRadiusSlider->setValue(static_cast<int>(r));
        this->updateMarkerRadius(r); // slider
      }
      else
      {
        this->markerRadiusSpinBox->setSuffix("");
        this->markerRadiusSpinBox->clear();
        //this->markerRadiusSlider->setValue(0);
        this->updateMarkerRadius(0.0); // slider
      }
      this->markerColorButton->setProperty("markerColor", uniqueColor ? c : QColor::Invalid);
      this->markerColorButton->setIcon(this->createMarkerIcon(uniqueColor ? c : Qt::transparent, true, uniqueColor ? true : false));
      this->mp_SetMarkerColorInHistory1->setEnabled(uniqueColor ? true : false);
      this->mp_SetMarkerColorInHistory2->setEnabled(uniqueColor ? true : false);
      this->mp_SetMarkerColorInHistory3->setEnabled(uniqueColor ? true : false);
      this->mp_SetMarkerColorInHistory4->setEnabled(uniqueColor ? true : false);
      this->mp_SetMarkerColorInHistory5->setEnabled(uniqueColor ? true : false);
      this->markerVisibilityCheckBox->setCheckState(visible);
      this->markerTrajectoryVisibilityCheckBox->setCheckState(trajectoryVisible);
    }
    this->markerVisibilityCheckBox->blockSignals(false);
    this->markerTrajectoryVisibilityCheckBox->blockSignals(false);
    this->markerRadiusSpinBox->blockSignals(false);
    static_cast<QGridLayout*>(this->markerPage->layout())->addWidget(this->frame,2,1);
    this->propertiesStack->setCurrentWidget(this->markerPage);
    break;
    }
  case PointType:
    {
    int id = items.first()->data(0,PointId).toInt();
    this->pointLabelEdit->setText(multipleSelection ? "" : this->mp_Acquisition->pointLabel(id));
    this->pointLabelEdit->setEnabled(!multipleSelection);
    this->pointDescEdit->setText(multipleSelection ? "" : this->mp_Acquisition->pointDescription(id));
    this->propertiesStack->setCurrentWidget(this->pointPage);
    break;
    }
  case AnalogType:
    {
    int id = items.first()->data(0,AnalogId).toInt();
    this->analogLabelEdit->setText(multipleSelection ? "" : this->mp_Acquisition->analogLabel(id));
    this->analogLabelEdit->setEnabled(!multipleSelection);
    this->analogDescEdit->setText(multipleSelection ? "" : this->mp_Acquisition->analogDescription(id));
    bool uniqueUnit = true, uniqueGain = true, uniqueOffset = true, uniqueScale = true;
    QString u = this->mp_Acquisition->analogUnit(id);
    Analog::Gain g = this->mp_Acquisition->analogGain(id);
    int o = this->mp_Acquisition->analogOffset(id);
    double s = this->mp_Acquisition->analogScale(id);
    QList<QTreeWidgetItem*>::const_iterator it = items.begin(); ++it;
    while (it != items.end())
    {
      id = (*it)->data(0,AnalogId).toInt();
      if (this->mp_Acquisition->analogUnit(id).compare(u) != 0)
        uniqueUnit = false;
      if (this->mp_Acquisition->analogGain(id) != g)
        uniqueGain = false;
      if (this->mp_Acquisition->analogOffset(id) != o)
        uniqueOffset = false;
      if (this->mp_Acquisition->analogScale(id) != s)
        uniqueScale = false;
      ++it;
    }
    this->analogUnitEdit->setText(uniqueUnit ? u : "");
    this->analogGainComboBox->blockSignals(true);
    this->analogGainComboBox->setCurrentIndex(uniqueGain ? g : 0);
    this->analogGainComboBox->blockSignals(false);
    this->analogScaleSpinBox->setDecimals(5);
    if (uniqueScale && (s < 1.0e5))
      this->analogScaleSpinBox->setDecimals(15);
    this->analogScaleSpinBox->setValue(uniqueScale ? s : 0.0); if (!uniqueOffset) this->analogScaleSpinBox->clear();
    this->analogOffsetSpinBox->setValue(uniqueOffset ? o : 0); if (!uniqueOffset) this->analogOffsetSpinBox->clear();
    this->propertiesStack->setCurrentWidget(this->analogPage);
    break;
    }
  case VideoType:
    {
    int id = items.first()->data(0,VideoId).toInt();
    this->videoFilenameEdit->setText(multipleSelection ? "" : this->mp_Acquisition->videoFilename(id));
    bool uniqueDelay = true;
    qint64 d = this->mp_Acquisition->videoDelay(id);
    QList<QTreeWidgetItem*>::const_iterator it = items.begin(); ++it;
    while (it != items.end())
    {
      id = (*it)->data(0,VideoId).toInt();
      if (this->mp_Acquisition->videoDelay(id) != d)
      {
        uniqueDelay = false;
        break;
      }
      ++it;
    }
    this->videoDelaySpinBox->setValue(uniqueDelay ? static_cast<double>(d) / 1000.0 : 0.0); if (!uniqueDelay) this->videoDelaySpinBox->clear();
    this->propertiesStack->setCurrentWidget(this->videoPage);
    break;
    }
  default:
    this->propertiesStack->setCurrentWidget(this->emptyPage);
    break;
  }
};

void ModelDockWidget::sendHiddenSegments()
{
  QList<int> ids;
  QTreeWidgetItem* segmentsRoot = this->modelTree->topLevelItem(SegmentsItem);
  for (int i = 0 ; i < segmentsRoot->childCount() ; ++i)
  {
    QTreeWidgetItem* item = segmentsRoot->child(i);
    if ((item->checkState(VisibleHeader) == Qt::Unchecked) || item->isHidden())
      ids << item->data(0,SegmentId).toInt();
  }
  emit segmentHiddenSelectionChanged(ids);
};

void ModelDockWidget::sendSelectedMarkers()
{
  QList<int> ids;
  QList<QTreeWidgetItem*> items = this->modelTree->selectedItems();
  for (QList<QTreeWidgetItem*>::const_iterator it = items.begin() ; it != items.end() ; ++it)
  {
    if ((*it)->type() == MarkerType)
      ids << (*it)->data(0,PointId).toInt();
  }
  emit markerSelectionChanged(ids);
};

void ModelDockWidget::sendHiddenMarkers()
{
  QList<int> ids;
  QTreeWidgetItem* markersRoot = this->modelTree->topLevelItem(MarkersItem);
  for (int i = 0 ; i < markersRoot->childCount() ; ++i)
  {
    QTreeWidgetItem* item = markersRoot->child(i);
    if ((item->checkState(VisibleHeader) == Qt::Unchecked) || item->isHidden())
      ids << item->data(0,PointId).toInt();
  }
  QTreeWidgetItem* virtualMarkersRoot = this->modelTree->topLevelItem(VirtualMarkersItem);
  for (int i = 0 ; i < virtualMarkersRoot->childCount() ; ++i)
  {
    QTreeWidgetItem* item = virtualMarkersRoot->child(i);
    if ((item->checkState(VisibleHeader) == Qt::Unchecked) || item->isHidden())
      ids << item->data(0,PointId).toInt();
  }
  emit markerHiddenSelectionChanged(ids);
};

void ModelDockWidget::sendTrackedMarkers()
{
  QList<int> ids;
  QTreeWidgetItem* markersRoot = this->modelTree->topLevelItem(MarkersItem);
  for (int i = 0 ; i < markersRoot->childCount() ; ++i)
  {
    QTreeWidgetItem* item = markersRoot->child(i);
    if (item->checkState(TrajectoryHeader) == Qt::Checked)
      ids << item->data(0,PointId).toInt();
  }
  QTreeWidgetItem* virtualMarkersRoot = this->modelTree->topLevelItem(VirtualMarkersItem);
  for (int i = 0 ; i < virtualMarkersRoot->childCount() ; ++i)
  {
    QTreeWidgetItem* item = virtualMarkersRoot->child(i);
    if (item->checkState(TrajectoryHeader) == Qt::Checked)
      ids << item->data(0,PointId).toInt();
  }
  emit markerTrajectorySelectionChanged(ids);
};

void ModelDockWidget::sendTrackedGRFPaths()
{
  QList<int> ids;
  QTreeWidgetItem* forcePlatesRoot = this->modelTree->topLevelItem(ForcePlatesItem);
  for (int i = 0 ; i < forcePlatesRoot->childCount() ; ++i)
  {
    QTreeWidgetItem* item = forcePlatesRoot->child(i)->child(2); // Position
    if (item->checkState(TrajectoryHeader) == Qt::Checked)
      ids << (item->data(0,ForcePlateId).toInt() - 65535) / 3; // 65535: because force platform ID starts from 65535 ; 3: because each component of the wrench has also a unique ID.
  }
  emit wrenchPositionSelectionChanged(ids);
};

void ModelDockWidget::sendModifiedMarkersState(QTreeWidgetItem* item, int column)
{
  Q_UNUSED(item);
  if (column == VisibleHeader)
  {
    if (item->type() == SegmentType)
      this->sendHiddenSegments();
    else //if (item->type() == MarkerType)
      this->sendHiddenMarkers();
  }
  else if (column == TrajectoryHeader)
  {
    this->sendTrackedMarkers();
    this->sendTrackedGRFPaths();
  }
};

void ModelDockWidget::editMarkerLabel()
{
  int id = this->modelTree->currentItem()->data(0, PointId).toInt();
  QString label = this->markerLabelEdit->text();
  if (label.compare(this->mp_Acquisition->pointLabel(id)) != 0)
    emit markerLabelChanged(id, label);
}

void ModelDockWidget::editMarkersDescription()
{
  QList<QTreeWidgetItem*> items = this->modelTree->selectedItems();
  QVector<int> ids = QVector<int>(items.count());
  int inc = 0;
  QString desc = this->markerDescEdit->text();
  bool descModified = false;
  for (QList<QTreeWidgetItem*>::const_iterator it = items.begin() ; it != items.end() ; ++it)
  {
    ids[inc] = (*it)->data(0, PointId).toInt();
    if (this->mp_Acquisition->pointDescription(ids[inc]).compare(desc) != 0)
      descModified = true;
    ++inc;
  }
  if (descModified)
    emit markersDescriptionChanged(ids, desc);
}

void ModelDockWidget::updateMarkerRadiusSpinBox(int v)
{
  this->markerRadiusSpinBox->setValue(static_cast<double>(v));
};

void ModelDockWidget::updateMarkerRadius(double r)
{
  this->markerRadiusSlider->blockSignals(true);
  this->markerRadiusSlider->setValue(static_cast<int>(r));
  this->markerRadiusSlider->blockSignals(false);
};

void ModelDockWidget::editMarkersRadius()
{
  QList<QTreeWidgetItem*> items = this->modelTree->selectedItems();
  QVector<int> ids = QVector<int>(items.count());
  int inc = 0;
  double radius = this->markerRadiusSpinBox->value();
  bool radiusModified = false;
  for (QList<QTreeWidgetItem*>::const_iterator it = items.begin() ; it != items.end() ; ++it)
  {
    ids[inc] = (*it)->data(0, PointId).toInt();
    if (this->mp_Acquisition->markerRadius(ids[inc]) != radius)
      radiusModified = true;
    ++inc;
  }
  if (radiusModified)
    emit markersRadiusChanged(ids, radius);
};

void ModelDockWidget::setMarkersRadius(const QVector<int>& ids, const QVector<double>& radii)
{
  Q_UNUSED(ids);
  Q_UNUSED(radii);
  this->displayProperties();
};

void ModelDockWidget::editMarkersVisibility()
{
  this->modelTree->blockSignals(true);
  QList<QTreeWidgetItem*> items = this->modelTree->selectedItems();
  QVector<int> ids = QVector<int>(items.count());
  int inc = 0;
  bool checked = this->markerVisibilityCheckBox->checkState() == Qt::Checked;
  bool visibilityModified = false;
  for (QList<QTreeWidgetItem*>::const_iterator it = items.begin() ; it != items.end() ; ++it)
  {
    (*it)->setCheckState(VisibleHeader, checked ? Qt::Checked : Qt::Unchecked);
    ids[inc] = (*it)->data(0, PointId).toInt();
    if (this->mp_Acquisition->markerVisible(ids[inc]) != checked)
      visibilityModified = true;
    ++inc;
  }
  this->modelTree->blockSignals(false);
  if (visibilityModified)
    emit markersVisibilityChanged(ids, checked);
};

void ModelDockWidget::setMarkersVisibility(const QVector<int>& ids, const QVector<bool>& visibles)
{
  this->modelTree->blockSignals(true);
  QList<QTreeWidgetItem*> roots;
  roots << this->modelTree->topLevelItem(MarkersItem);
  roots << this->modelTree->topLevelItem(VirtualMarkersItem);
  for (int i = 0 ; i < ids.size() ; ++i)
  {
    for (QList<QTreeWidgetItem*>::iterator itR = roots.begin() ; itR != roots.end() ; ++itR)
    {
      bool found = false;
      for (int j = 0 ; j < (*itR)->childCount() ; ++j)
      {
        QTreeWidgetItem* item = (*itR)->child(j);
        if (item->data(0, PointId).toInt() == ids[i])
        {
          item->setCheckState(VisibleHeader, visibles[i] ? Qt::Checked : Qt::Unchecked);
          found = true;
          break;
        }
      }
      if (found) break;
    }
  }
  this->modelTree->blockSignals(false);
  this->displayProperties();
};

void ModelDockWidget::editMarkersTrajectoryVisibility()
{
  this->modelTree->blockSignals(true);
  QList<QTreeWidgetItem*> items = this->modelTree->selectedItems();
  QVector<int> ids = QVector<int>(items.count());
  int inc = 0;
  bool checked = this->markerTrajectoryVisibilityCheckBox->checkState() == Qt::Checked;
  bool visibilityModified = false;
  for (QList<QTreeWidgetItem*>::const_iterator it = items.begin() ; it != items.end() ; ++it)
  {
    (*it)->setCheckState(TrajectoryHeader, checked ? Qt::Checked : Qt::Unchecked);
    ids[inc] = (*it)->data(0, PointId).toInt();
    if (this->mp_Acquisition->markerTrajectoryVisible(ids[inc]) != checked)
      visibilityModified = true;
    ++inc;
  }
  this->modelTree->blockSignals(false);
  if (visibilityModified)
    emit markersTrajectoryVisibilityChanged(ids, checked);
};

void ModelDockWidget::setMarkersTrajectoryVisibility(const QVector<int>& ids, const QVector<bool>& visibles)
{
  this->modelTree->blockSignals(true);
  QList<QTreeWidgetItem*> roots;
  roots << this->modelTree->topLevelItem(MarkersItem);
  roots << this->modelTree->topLevelItem(VirtualMarkersItem);
  for (int i = 0 ; i < ids.size() ; ++i)
  {
    for (QList<QTreeWidgetItem*>::iterator itR = roots.begin() ; itR != roots.end() ; ++itR)
    {
      bool found = false;
      for (int j = 0 ; j < (*itR)->childCount() ; ++j)
      {
        QTreeWidgetItem* item = (*itR)->child(j);
        if (item->data(0, PointId).toInt() == ids[i])
        {
          item->setCheckState(TrajectoryHeader, visibles[i] ? Qt::Checked : Qt::Unchecked);
          found = true;
          break;
        }
      }
      if (found) break;
    }
  }
  this->modelTree->blockSignals(false);
  this->displayProperties();
};

void ModelDockWidget::setMarkersConfiguration(const QList<int>& ids, const QList<bool>& visibles, const QList<bool>& trajectories, const QList<double>& radii, const QList<QColor>& colors)
{
  Q_UNUSED(radii);
  
  this->modelTree->blockSignals(true);
  QList<QTreeWidgetItem*> roots;
  roots << this->modelTree->topLevelItem(MarkersItem);
  roots << this->modelTree->topLevelItem(VirtualMarkersItem);
  for (int i = 0 ; i < ids.size() ; ++i)
  {
    for (QList<QTreeWidgetItem*>::iterator itR = roots.begin() ; itR != roots.end() ; ++itR)
    {
      bool found = false;
      for (int j = 0 ; j < (*itR)->childCount() ; ++j)
      {
        QTreeWidgetItem* item = (*itR)->child(j);
        if (item->data(0, PointId).toInt() == ids[i])
        {
          item->setIcon(0, this->createMarkerIcon(colors[i]));
          item->setCheckState(VisibleHeader, visibles[i] ? Qt::Checked : Qt::Unchecked);
          item->setCheckState(TrajectoryHeader, trajectories[i] ? Qt::Checked : Qt::Unchecked);
          found = true;
          break;
        }
      }
      if (found) break;
    }
  }
  this->modelTree->blockSignals(false);
  this->displayProperties();
};

void ModelDockWidget::editMarkersColor()
{
  QColor color = QColorDialog::getColor(Qt::white, this);
  if (color.isValid())
    this->editMarkersColor(color);
};

void ModelDockWidget::setMarkerColorFromRecent1()
{
  this->editMarkersColor(this->m_RecentColors[0]);
};

void ModelDockWidget::setMarkerColorFromRecent2()
{
  this->editMarkersColor(this->m_RecentColors[1]);
};

void ModelDockWidget::setMarkerColorFromRecent3()
{
  this->editMarkersColor(this->m_RecentColors[2]);
};

void ModelDockWidget::setMarkerColorFromRecent4()
{
  this->editMarkersColor(this->m_RecentColors[3]);
};

void ModelDockWidget::setMarkerColorFromRecent5()
{
  this->editMarkersColor(this->m_RecentColors[4]);
};

void ModelDockWidget::resetMarkerRecentColors()
{
  this->m_RecentColors.clear();
  this->m_RecentColors.resize(5);
  this->drawRecentColors();
};

void ModelDockWidget::setMarkerRecentColor1()
{
  this->setRecentColor(0, this->markerColorButton->property("markerColor").value<QColor>());
};

void ModelDockWidget::setMarkerRecentColor2()
{
  this->setRecentColor(1, this->markerColorButton->property("markerColor").value<QColor>());
};

void ModelDockWidget::setMarkerRecentColor3()
{
  this->setRecentColor(2, this->markerColorButton->property("markerColor").value<QColor>());
};

void ModelDockWidget::setMarkerRecentColor4()
{
  this->setRecentColor(3, this->markerColorButton->property("markerColor").value<QColor>());
};

void ModelDockWidget::setMarkerRecentColor5()
{
  this->setRecentColor(4, this->markerColorButton->property("markerColor").value<QColor>());
};

void ModelDockWidget::editPointLabel()
{
  int id = this->modelTree->currentItem()->data(0, PointId).toInt();
  QString label = this->pointLabelEdit->text();
  if (label.compare(this->mp_Acquisition->pointLabel(id)) != 0)
    emit pointLabelChanged(id, label);
}

void ModelDockWidget::setPointLabel(int id, const QString& label)
{
  // Try with markers
  QTreeWidgetItem* markersRoot = this->modelTree->topLevelItem(MarkersItem);
  for (int i = 0 ; i < markersRoot->childCount() ; ++i)
  {
    if (markersRoot->child(i)->data(0, PointId).toInt() == id)
    {
      markersRoot->child(i)->setText(0, label);
      this->displayProperties();
      return;
    }
  }
  // Virtual markers
  QTreeWidgetItem* virtualMarkersRoot = this->modelTree->topLevelItem(VirtualMarkersItem);
  for (int i = 0 ; i < virtualMarkersRoot->childCount() ; ++i)
  {
    if (virtualMarkersRoot->child(i)->data(0, PointId).toInt() == id)
    {
      virtualMarkersRoot->child(i)->setText(0, label);
      this->displayProperties();
      return;
    }
  }
  // Other points
  QTreeWidgetItem* modelOutputsRoot = this->modelTree->topLevelItem(ModelOutputsItem);
  for (int i = 0 ; i < modelOutputsRoot->childCount() ; ++i)
  {
    QTreeWidgetItem* child = modelOutputsRoot->child(i);
    for (int j = 0 ; j < child->childCount() ; ++j)
    {
      if (child->child(j)->data(0, PointId).toInt() == id)
      {
        child->child(j)->setText(0, label);
        this->displayProperties();
        return;
      }
    }
  }
};

void ModelDockWidget::editPointsDescription()
{
  QList<QTreeWidgetItem*> items = this->modelTree->selectedItems();
  QVector<int> ids = QVector<int>(items.count());
  int inc = 0;
  QString desc = this->pointDescEdit->text();
  bool descModified = false;
  for (QList<QTreeWidgetItem*>::const_iterator it = items.begin() ; it != items.end() ; ++it)
  {
    ids[inc] = (*it)->data(0, PointId).toInt();
    if (this->mp_Acquisition->pointDescription(ids[inc]).compare(desc) != 0)
      descModified = true;
    ++inc;
  }
  if (descModified)
    emit pointsDescriptionChanged(ids, desc);
}

void ModelDockWidget::setPointsDescription(const QVector<int>& ids, const QVector<QString>& descs)
{
  Q_UNUSED(ids);
  Q_UNUSED(descs);
  this->displayProperties();
};

void ModelDockWidget::removePoints(const QList<int>& ids, const QList<Point*>& points)
{
  this->modelTree->blockSignals(true);
  bool itemUnselected = false;
  QTreeWidgetItem* markersRoot = this->modelTree->topLevelItem(MarkersItem);
  QTreeWidgetItem* virtualMarkersRoot = this->modelTree->topLevelItem(VirtualMarkersItem);
  QTreeWidgetItem* modelOutputsRoot = this->modelTree->topLevelItem(ModelOutputsItem);
  QTreeWidgetItem* modelOutputAngles = modelOutputsRoot->child(0);
  QTreeWidgetItem* modelOutputForces = modelOutputsRoot->child(1);
  QTreeWidgetItem* modelOutputMoments = modelOutputsRoot->child(2);
  QTreeWidgetItem* modelOutputPowers = modelOutputsRoot->child(3);
  QTreeWidgetItem* modelOutputScalars = modelOutputsRoot->child(4);
  for (int i = 0 ; i < ids.count() ; ++i)
  {
    QTreeWidgetItem* item = 0;
    switch (points[i]->type)
    {
    case Point::Marker:
      item = this->treePointChild(markersRoot, ids[i]);
      break;
    case Point::VirtualMarker:
    case Point::VirtualMarkerForFrame:
      item = this->treePointChild(virtualMarkersRoot, ids[i]);
      break;
    case Point::Angle:
      item = this->treePointChild(modelOutputAngles, ids[i]);
      break;
    case Point::Force:
      item = this->treePointChild(modelOutputForces, ids[i]);
      break;
    case Point::Moment:
      item = this->treePointChild(modelOutputMoments, ids[i]);
      break;
    case Point::Power:
      item = this->treePointChild(modelOutputPowers, ids[i]);
      break;
    case Point::Scalar:
      item = this->treePointChild(modelOutputScalars, ids[i]);
      break;
    default:
      break;
    }
    if (item != 0)
    {
      if (item->isSelected())
      {
        this->modelTree->setCurrentItem(item, 0, QItemSelectionModel::Deselect);
        this->modelTree->setCurrentItem(item, 1, QItemSelectionModel::Deselect);
        this->modelTree->setCurrentItem(item, 2, QItemSelectionModel::Deselect);
        itemUnselected = true;
      }
      item->setHidden(true);
    }
  }
  this->modelTree->blockSignals(false);
  this->sendHiddenMarkers();
  this->refresh();
  if (itemUnselected)
  {
    this->pointLabelEdit->blockSignals(true);
    this->pointDescEdit->blockSignals(true);
    this->markerLabelEdit->blockSignals(true);
    this->markerRadiusSpinBox->blockSignals(true);
    this->markerDescEdit->blockSignals(true);
    this->displayProperties();
    this->pointLabelEdit->blockSignals(false);
    this->pointDescEdit->blockSignals(false);
    this->markerLabelEdit->blockSignals(false);
    this->markerRadiusSpinBox->blockSignals(false);
    this->markerDescEdit->blockSignals(false);
  }
};

// TODO: Think about the case where points are created instead of hidden!
void ModelDockWidget::insertPoints(const QList<int>& ids, const QList<Point*>& points)
{
  Q_UNUSED(points);
  QTreeWidgetItem* markersRoot = this->modelTree->topLevelItem(MarkersItem);
  QTreeWidgetItem* virtualMarkersRoot = this->modelTree->topLevelItem(VirtualMarkersItem);
  QTreeWidgetItem* modelOutputsRoot = this->modelTree->topLevelItem(ModelOutputsItem);
  QTreeWidgetItem* modelOutputAngles = modelOutputsRoot->child(0);
  QTreeWidgetItem* modelOutputForces = modelOutputsRoot->child(1);
  QTreeWidgetItem* modelOutputMoments = modelOutputsRoot->child(2);
  QTreeWidgetItem* modelOutputPowers = modelOutputsRoot->child(3);
  QTreeWidgetItem* modelOutputScalars = modelOutputsRoot->child(4);
  for (int i = 0 ; i < ids.count() ; ++i)
  {
    switch (points[i]->type)
    {
    case Point::Marker:
      this->treePointChild(markersRoot, ids[i])->setHidden(false);
      break;
    case Point::VirtualMarker:
    case Point::VirtualMarkerForFrame:
      this->treePointChild(virtualMarkersRoot, ids[i])->setHidden(false);
      break;
    case Point::Angle:
      this->treePointChild(modelOutputAngles, ids[i])->setHidden(false);
      break;
    case Point::Force:
      this->treePointChild(modelOutputForces, ids[i])->setHidden(false);
      break;
    case Point::Moment:
      this->treePointChild(modelOutputMoments, ids[i])->setHidden(false);
      break;
    case Point::Power:
      this->treePointChild(modelOutputPowers, ids[i])->setHidden(false);
      break;
    case Point::Scalar:
      this->treePointChild(modelOutputScalars, ids[i])->setHidden(false);
      break;
    default:
      break;
    }
  }
  this->sendHiddenMarkers();
  this->refresh();
};

void ModelDockWidget::editAnalogLabel()
{
  int id = this->modelTree->currentItem()->data(0, AnalogId).toInt();
  QString label = this->analogLabelEdit->text();
  if (label.compare(this->mp_Acquisition->analogLabel(id)) != 0)
    emit analogLabelChanged(id, label);
};

void ModelDockWidget::setAnalogLabel(int id, const QString& label)
{
  QTreeWidgetItem* analogsRoot = this->modelTree->topLevelItem(AnalogsItem);
  for (int i = 0 ; i < analogsRoot->childCount() ; ++i)
  {
    if (analogsRoot->child(i)->data(0, AnalogId).toInt() == id)
    {
      analogsRoot->child(i)->setText(0, label);
      break;
    }
  }
  this->displayProperties();
};

void ModelDockWidget::editAnalogsUnit()
{
  QList<QTreeWidgetItem*> items = this->modelTree->selectedItems();
  QVector<int> ids = QVector<int>(items.count());
  int inc = 0;
  QString unit = this->analogUnitEdit->text();
  bool unitModified = false;
  for (QList<QTreeWidgetItem*>::const_iterator it = items.begin() ; it != items.end() ; ++it)
  {
    ids[inc] = (*it)->data(0, AnalogId).toInt();
    if (this->mp_Acquisition->analogUnit(ids[inc]).compare(unit) != 0)
      unitModified = true;
    ++inc;
  }
  if (unitModified)
    emit analogsUnitChanged(ids, unit);
};

void ModelDockWidget::setAnalogsUnit(const QVector<int>& ids, const QVector<QString>& units)
{
  Q_UNUSED(ids);
  Q_UNUSED(units);
  this->displayProperties();
};

void ModelDockWidget::editAnalogsGain(int index)
{
  if (!this->isOkToModifyAnalog())
  {
    this->displayProperties();
    return;
  }
  QList<QTreeWidgetItem*> items = this->modelTree->selectedItems();
  QVector<int> ids = QVector<int>(items.count());
  int inc = 0;
  for (QList<QTreeWidgetItem*>::const_iterator it = items.begin() ; it != items.end() ; ++it)
    ids[inc++] = (*it)->data(0, AnalogId).toInt();    
  emit analogsGainChanged(ids, static_cast<Analog::Gain>(index));
};

void ModelDockWidget::setAnalogsGain(const QVector<int>& ids, const QVector<Analog::Gain>& gains)
{
  Q_UNUSED(ids);
  Q_UNUSED(gains);
  this->displayProperties();
};

void ModelDockWidget::editAnalogsOffset()
{
  QList<QTreeWidgetItem*> items = this->modelTree->selectedItems();
  QVector<int> ids = QVector<int>(items.count());
  int inc = 0;
  int offset = this->analogOffsetSpinBox->value();
  bool offsetModified = false;
  for (QList<QTreeWidgetItem*>::const_iterator it = items.begin() ; it != items.end() ; ++it)
  {
    ids[inc] = (*it)->data(0, AnalogId).toInt();
    if (this->mp_Acquisition->analogOffset(ids[inc]) != offset)
      offsetModified = true;
    ++inc;
  }
  if (offsetModified)
  {
    this->analogOffsetSpinBox->blockSignals(true);
    if (!this->isOkToModifyAnalog())
    {
      this->analogOffsetSpinBox->clear();
      this->displayProperties();
    }
    else
      emit analogsOffsetChanged(ids, offset);
    this->analogOffsetSpinBox->blockSignals(false);
  }
};

void ModelDockWidget::setAnalogsOffset(const QVector<int>& ids, const QVector<int>& offsets)
{
  Q_UNUSED(ids);
  Q_UNUSED(offsets);
  this->displayProperties();
};

void ModelDockWidget::editAnalogsScale()
{
  QList<QTreeWidgetItem*> items = this->modelTree->selectedItems();
  QVector<int> ids = QVector<int>(items.count());
  int inc = 0;
  double scale = this->analogScaleSpinBox->value();
  bool scaleModified = false;
  for (QList<QTreeWidgetItem*>::const_iterator it = items.begin() ; it != items.end() ; ++it)
  {
    ids[inc] = (*it)->data(0, AnalogId).toInt();
    if (fabs(this->mp_Acquisition->analogScale(ids[inc]) - scale) >= 5.0e-6)
      scaleModified = true;
    ++inc;
  }
  if (scaleModified)
  {
    this->analogScaleSpinBox->blockSignals(true);
    if (!this->isOkToModifyAnalog())
    {
      this->analogScaleSpinBox->clear();
      this->displayProperties();
    }
    else
      emit analogsScaleChanged(ids, scale);
    this->analogScaleSpinBox->blockSignals(false);
  }
};

void ModelDockWidget::setAnalogsScale(const QVector<int>& ids, const QVector<double>& scales)
{
  Q_UNUSED(ids);
  Q_UNUSED(scales);
  this->displayProperties();
};

void ModelDockWidget::editAnalogsDescription()
{
  QList<QTreeWidgetItem*> items = this->modelTree->selectedItems();
  QVector<int> ids = QVector<int>(items.count());
  int inc = 0;
  QString desc = this->analogDescEdit->text();
  bool descModified = false;
  for (QList<QTreeWidgetItem*>::const_iterator it = items.begin() ; it != items.end() ; ++it)
  {
    ids[inc] = (*it)->data(0, AnalogId).toInt();
    if (this->mp_Acquisition->analogDescription(ids[inc]).compare(desc) != 0)
      descModified = true;
    ++inc;
  }
  if (descModified)
    emit analogsDescriptionChanged(ids, desc);
};

void ModelDockWidget::setAnalogsDescription(const QVector<int>& ids, const QVector<QString>& descs)
{
  Q_UNUSED(ids);
  Q_UNUSED(descs);
  this->displayProperties();
};

void ModelDockWidget::removeAnalogs(const QList<int>& ids, const QList<Analog*>& analogs)
{
  Q_UNUSED(analogs);
  this->modelTree->blockSignals(true);
  bool itemUnselected = false;
  QTreeWidgetItem* analogsRoot = this->modelTree->topLevelItem(AnalogsItem);
  for (int i = 0 ; i < ids.count() ; ++i)
  {
    QTreeWidgetItem* item = analogsRoot->child(ids[i]);
    if (item->isSelected())
    {
      this->modelTree->setCurrentItem(item, 0, QItemSelectionModel::Deselect);
      this->modelTree->setCurrentItem(item, 1, QItemSelectionModel::Deselect);
      this->modelTree->setCurrentItem(item, 2, QItemSelectionModel::Deselect);
      itemUnselected = true;
    }
    item->setHidden(true);
  }
  this->modelTree->blockSignals(false);
  this->refresh();
  if (itemUnselected)
  {
    this->analogLabelEdit->blockSignals(true);
    this->analogUnitEdit->blockSignals(true);
    this->analogScaleSpinBox->blockSignals(true);
    this->analogOffsetSpinBox->blockSignals(true);
    this->analogDescEdit->blockSignals(true);
    this->displayProperties();
    this->analogLabelEdit->blockSignals(false);
    this->analogUnitEdit->blockSignals(false);
    this->analogScaleSpinBox->blockSignals(false);
    this->analogOffsetSpinBox->blockSignals(false);
    this->analogDescEdit->blockSignals(false);
  }
};

// TODO: Think about the case where analog channels are created instead of hidden!
void ModelDockWidget::insertAnalogs(const QList<int>& ids, const QList<Analog*>& analogs)
{
  Q_UNUSED(analogs);
  QTreeWidgetItem* analogsRoot = this->modelTree->topLevelItem(AnalogsItem);
  for (int i = 0 ; i < ids.count() ; ++i)
    analogsRoot->child(ids[i])->setHidden(false);
  this->refresh();
};

void ModelDockWidget::showGroundRectionForcePaths()
{
  this->setGroundRectionForcePathsVisibility(true);
};

void ModelDockWidget::hideGroundRectionForcePaths()
{
  this->setGroundRectionForcePathsVisibility(false);
};

void ModelDockWidget::setGroundRectionForcePathsVisibility(bool visible)
{
  QList<int> ids;
  QTreeWidgetItem* forcePlatesRoot = this->modelTree->topLevelItem(ForcePlatesItem);
  this->modelTree->blockSignals(true);
  for (int i = 0 ; i < forcePlatesRoot->childCount() ; ++i)
  {
    QTreeWidgetItem* item = forcePlatesRoot->child(i)->child(2); // Position
    item->setCheckState(TrajectoryHeader, visible ? Qt::Checked : Qt::Unchecked);
    if (visible)
      ids << (item->data(0,ForcePlateId).toInt() - 65535) / 3; // 65535: because force platform ID starts from 65535 ; 3: because each component of the wrench has also a unique ID.
  }
  this->modelTree->blockSignals(false);
  emit wrenchPositionSelectionChanged(ids);
};

void ModelDockWidget::editVideosDelay()
{
  QList<QTreeWidgetItem*> items = this->modelTree->selectedItems();
  QVector<int> ids = QVector<int>(items.count());
  int inc = 0;
  qint64 delay = static_cast<qint64>(this->videoDelaySpinBox->value() * 1000.0);
  bool delayModified = false;
  for (QList<QTreeWidgetItem*>::const_iterator it = items.begin() ; it != items.end() ; ++it)
  {
    ids[inc] = (*it)->data(0, VideoId).toInt();
    if (this->mp_Acquisition->videoDelay(ids[inc]) != delay)
      delayModified = true;
    ++inc;
  }
  if (delayModified)
    emit videosDelayChanged(ids, delay);
};

void ModelDockWidget::setVideosDelay(const QVector<int>& ids, const QVector<qint64>& delays)
{
  Q_UNUSED(ids);
  Q_UNUSED(delays);
  this->displayProperties();
};

void ModelDockWidget::updateVideoDelaySpinBox(int d)
{
  this->videoDelaySpinBox->setValue(static_cast<double>(d) / 10.0);
};

void ModelDockWidget::updateVideoDelay(double d)
{
  this->videoDelaySlider->blockSignals(true);
  this->videoDelaySlider->setValue(static_cast<int>(d * 10.0));
  this->videoDelaySlider->blockSignals(false);
};

void ModelDockWidget::removeVideos(const QList<int>& ids, const QList<Video*>& videos)
{
  Q_UNUSED(videos);
  this->modelTree->blockSignals(true);
  bool itemUnselected = false;
  QTreeWidgetItem* vdeosRoot = this->modelTree->topLevelItem(VideosItem);
  for (int i = 0 ; i < ids.count() ; ++i)
  {
    QTreeWidgetItem* item = vdeosRoot->child(ids[i]);
    if (item->isSelected())
    {
      this->modelTree->setCurrentItem(item, 0, QItemSelectionModel::Deselect);
      this->modelTree->setCurrentItem(item, 1, QItemSelectionModel::Deselect);
      this->modelTree->setCurrentItem(item, 2, QItemSelectionModel::Deselect);
      itemUnselected = true;
    }
    item->setHidden(true);
  }
  this->modelTree->blockSignals(false);
  this->refresh();
  if (itemUnselected)
  {
    this->videoFilenameEdit->blockSignals(true);
    this->videoDelaySpinBox->blockSignals(true);
    this->displayProperties();
    this->videoFilenameEdit->blockSignals(false);
    this->videoDelaySpinBox->blockSignals(false);
  }
};

void ModelDockWidget::insertVideos(const QList<int>& ids, const QList<Video*>& videos)
{
  Q_UNUSED(videos);
  QTreeWidgetItem* videosRoot = this->modelTree->topLevelItem(VideosItem);
  for (int i = 0 ; i < ids.count() ; ++i)
    videosRoot->child(ids[i])->setHidden(false);
  this->refresh();
};

void ModelDockWidget::importVideos(const QList<int>& ids, const QList<Video*>& videos)
{
  // Append new videos
  QTreeWidgetItem* videosRoot = this->modelTree->topLevelItem(VideosItem);
  for (int i = 0 ; i < ids.count() ; ++i)
    videosRoot->addChild(this->createVideoItem(videos[i]->label, ids[i], videos[i]->error));
  // Update the tree
  if ((videosRoot->childCount() != 0) && this->hasChildVisible(videosRoot))
    videosRoot->setHidden(false);
  else
    videosRoot->setHidden(true);
};

void ModelDockWidget::changeEvent(QEvent* event)
{
  if (event->type() == QEvent::EnabledChange)
  {
    if (this->isEnabled())
    {
      this->updateDisplayedMarkers(this->m_DisplayedIds);
    }
    else
    {
      QTreeWidgetItem* markersRoot = this->modelTree->topLevelItem(MarkersItem);
      for (int i = 0 ; i < markersRoot->childCount() ; ++i)
      {
        QTreeWidgetItem* item = markersRoot->child(i);
        if (item->foreground(0) == displayLabelColor)
          this->m_DisplayedIds[i] = 1;
        else
          this->m_DisplayedIds[i] = 0;
      }
      this->updateDisplayedMarkers(QVector<int>());
    }
  }
  this->QDockWidget::changeEvent(event);
};

void ModelDockWidget::setCurrentConfiguration(int idx)
{
  this->modelConfigurationComboBox->setCurrentIndex(idx);
  this->m_CurrentConfigurationIndex = idx;
};

bool ModelDockWidget::saveConfiguration(int idx)
{
  QString filename;
  if (this->m_ConfigurationItems[idx].isNew)
  {
    QFileInfo fileInfo(this->mp_Acquisition->fileName());
#if 1
    filename = QFileDialog::getSaveFileName(this,
                         tr("Save Model Visual Configuration"),
                         fileInfo.path() + "/" + this->m_ConfigurationItems[idx].name + ".mvc",
                         tr("Model Visual Configuration Files (*.mvc)"));
    if (filename.isEmpty())
      return false;
    else
      this->m_ConfigurationItems[idx].filename = filename;
#else
    QFileDialog* saveConfig = new QFileDialog(this);
    saveConfig->setFileMode(QFileDialog::AnyFile);
    saveConfig->setAcceptMode(QFileDialog::AcceptSave);
    saveConfig->setDirectory(fileInfo.dir());
    selectFile(fileName)
    saveConfig->setNameFilter(tr("Model Visual Configuration Files (*.mvc)"));
    saveConfig->setViewMode(QFileDialog::List);
    bool newFile = saveConfig->exec();
    delete saveConfig;
    if (newFile)
      this->m_ConfigurationItems[idx].filename = saveConfig->selectedFiles()[0];
    else
      return false;
#endif
  }
  else
    filename = this->m_ConfigurationItems[idx].filename;
  
  QMessageBox messageBox(QMessageBox::Warning, 
                         "",
                         tr("Cannot write file: "),
                         QMessageBox::Ok,
                         this->parentWidget());
  messageBox.setDefaultButton(QMessageBox::Ok);
#ifdef Q_OS_MAC
  messageBox.setWindowModality(Qt::WindowModal);
  messageBox.setWindowFlags(Qt::Sheet);
#endif
  QFile file(filename);
  if (!file.open(QFile::WriteOnly | QFile::Text))
  {
    LOG_CRITICAL("Error when writing the file: " + filename + ":" + file.errorString());
    messageBox.setText(messageBox.text() + filename);
    messageBox.setInformativeText(file.errorString());
    messageBox.exec();
    return false;
  }
  
  QList<QTreeWidgetItem*> roots;
  roots << this->modelTree->topLevelItem(MarkersItem);
  roots << this->modelTree->topLevelItem(VirtualMarkersItem);
  QTreeWidgetItem* segmentsRoot = this->modelTree->topLevelItem(SegmentsItem);
  
  QXmlStreamWriter xmlWriter(&file);
  xmlWriter.setAutoFormatting(true);
  xmlWriter.writeStartDocument();
  xmlWriter.writeStartElement("MokkaModelVisualConfiguration");
  xmlWriter.writeAttribute("name", this->m_ConfigurationItems[idx].name);
  xmlWriter.writeAttribute("version", "1.0");
  xmlWriter.writeStartElement("MarkersList");
  for (QList<QTreeWidgetItem*>::iterator itR = roots.begin() ; itR != roots.end() ; ++itR)
  {
    for (int i = 0 ; i < (*itR)->childCount() ; ++i)
    {
      QTreeWidgetItem* item = (*itR)->child(i);
      if (!item->isHidden())
      {
        int id = item->data(0,PointId).toInt();
        xmlWriter.writeStartElement("Marker");
        xmlWriter.writeAttribute("label", item->text(0));
        xmlWriter.writeAttribute("radius", QString::number(this->mp_Acquisition->markerRadius(id)));
        QColor c = this->mp_Acquisition->markerColor(id);
        xmlWriter.writeAttribute("colorR", QString::number(c.redF()));
        xmlWriter.writeAttribute("colorG", QString::number(c.greenF()));
        xmlWriter.writeAttribute("colorB", QString::number(c.blueF()));
        xmlWriter.writeAttribute("visible", this->mp_Acquisition->markerVisible(id) ? "1" : "0");
        xmlWriter.writeAttribute("trajectory", this->mp_Acquisition->markerTrajectoryVisible(id) ? "1" : "0");
        xmlWriter.writeEndElement(); // End Marker
      }
    }
  }
  xmlWriter.writeEndElement(); // End MarkersList
  xmlWriter.writeStartElement("SegmentsList");
  for (int i = 0 ; i < segmentsRoot->childCount() ; ++i)
  {
    QTreeWidgetItem* item = segmentsRoot->child(i);
    if (!item->isHidden())
    {
      int id = item->data(0,SegmentId).toInt();
      xmlWriter.writeStartElement("Segment");
      xmlWriter.writeAttribute("label", item->text(0));
      QColor c = this->mp_Model->segmentColor(id);
      xmlWriter.writeAttribute("colorR", QString::number(c.redF()));
      xmlWriter.writeAttribute("colorG", QString::number(c.greenF()));
      xmlWriter.writeAttribute("colorB", QString::number(c.blueF()));
      xmlWriter.writeAttribute("visible", this->mp_Model->segmentVisible(id) ? "1" : "0");
      xmlWriter.writeAttribute("surface", this->mp_Model->segmentSurfaceVisible(id) ? "1" : "0");
      xmlWriter.writeAttribute("description", this->mp_Model->segmentDescription(id));
      QVector<int> markerIds = this->mp_Model->segmentMarkerIds(id);
      for (int j = 0 ; j < markerIds.size() ; ++j)
      {
        xmlWriter.writeStartElement("Point");
        xmlWriter.writeAttribute("label", this->mp_Acquisition->pointLabel(markerIds[j]));
        xmlWriter.writeEndElement();
      }
      QVector<Pair> links = this->mp_Model->segmentLinks(id);
      for (int j = 0 ; j < links.size() ; ++j)
      {
        xmlWriter.writeStartElement("Link");
        xmlWriter.writeAttribute("pt1", this->mp_Acquisition->pointLabel(links[j].GetIds()[0]));
        xmlWriter.writeAttribute("pt2", this->mp_Acquisition->pointLabel(links[j].GetIds()[1]));
        xmlWriter.writeEndElement();
      }
      QVector<Triad> faces = this->mp_Model->segmentFaces(id);
      for (int j = 0 ; j < faces.size() ; ++j)
      {
        xmlWriter.writeStartElement("Face");
        xmlWriter.writeAttribute("pt1", this->mp_Acquisition->pointLabel(faces[j].GetIds()[0]));
        xmlWriter.writeAttribute("pt2", this->mp_Acquisition->pointLabel(faces[j].GetIds()[1]));
        xmlWriter.writeAttribute("pt3", this->mp_Acquisition->pointLabel(faces[j].GetIds()[2]));
        xmlWriter.writeEndElement();
      }
      xmlWriter.writeEndElement(); // End Segment
    }
  }
  xmlWriter.writeEndElement(); // End SegmentsList
  xmlWriter.writeEndElement(); // End MokkaModelVisualConfiguration
  xmlWriter.writeEndDocument();
  file.close();
  if (file.error())
  {
      messageBox.setText(messageBox.text() + filename + "\n" + file.errorString());
      messageBox.exec();
      return false;
  }
  
  emit configurationSaved();
  
  this->m_ConfigurationItems[idx].isNew = false;
  this->m_ConfigurationItems[idx].isModified = false;
  this->mp_SaveConfiguration->setEnabled(false);
  this->modelConfigurationComboBox->setItemText(idx, this->m_ConfigurationItems[idx].name);
  return true;
};

void ModelDockWidget::removeConfiguration(int idx)
{
  this->m_ConfigurationItems.removeAt(idx);
  this->modelConfigurationComboBox->blockSignals(true);
  this->modelConfigurationComboBox->removeItem(idx);
  this->modelConfigurationComboBox->setCurrentIndex(-1);
  this->mp_SaveConfiguration->setEnabled(false);
  this->mp_RemoveConfiguration->setEnabled(false);
  this->mp_DeselectConfiguration->setEnabled(false);
  this->modelConfigurationComboBox->blockSignals(false);
};

void ModelDockWidget::setConfigurationModified(int idx, bool modified)
{
  this->m_ConfigurationItems[idx].isModified = modified;
  this->modelConfigurationComboBox->setItemText(idx, modified ? "*" + this->m_ConfigurationItems[idx].name : this->m_ConfigurationItems[idx].name);  
  this->mp_SaveConfiguration->setEnabled(modified ? false : true);
};

QTreeWidgetItem* ModelDockWidget::createSegmentItem(const QString& label, int id, bool checked)
{
  QTreeWidgetItem* segmentItem = new QTreeWidgetItem(QStringList(label), SegmentType);
  segmentItem->setIcon(LabelHeader, this->createSegmentIcon(this->mp_Model->segmentColor(id)));
  segmentItem->setCheckState(VisibleHeader, checked ? Qt::Checked : Qt::Unchecked);
  segmentItem->setData(0, SegmentId, id);
  return segmentItem;
};

QTreeWidgetItem* ModelDockWidget::createMarkerItem(const QString& label, int id, bool checked)
{
  QTreeWidgetItem* markerItem = new QTreeWidgetItem(QStringList(label), MarkerType);
  markerItem->setIcon(LabelHeader, this->createMarkerIcon(this->mp_Acquisition->markerColor(id)));
  markerItem->setCheckState(VisibleHeader, checked ? Qt::Checked : Qt::Unchecked);
  markerItem->setCheckState(TrajectoryHeader, Qt::Unchecked);
  markerItem->setData(0, PointId, id);
  return markerItem;
}

QTreeWidgetItem* ModelDockWidget::createAnalogItem(const QString& label, int id)
{
  QTreeWidgetItem* analogItem = new QTreeWidgetItem(QStringList(label), AnalogType);
  analogItem->setIcon(LabelHeader, *this->mp_AnalogsIcon);
  analogItem->setData(0, AnalogId, id);
  return analogItem;
};

QTreeWidgetItem* ModelDockWidget::createModelOutputItem(const QString& label, int id)
{
  QTreeWidgetItem* modelOutputItem = new QTreeWidgetItem(QStringList(label), PointType);
  modelOutputItem->setIcon(LabelHeader, *this->mp_ModelOutputsIcon);
  modelOutputItem->setData(0, PointId, id);
  return modelOutputItem;
};

QTreeWidgetItem* ModelDockWidget::createVideoItem(const QString& label, int id, bool error)
{
  QTreeWidgetItem* videoItem = new QTreeWidgetItem(QStringList(label), VideoType);
  videoItem->setIcon(LabelHeader, !error ? *this->mp_VideosIcon : *this->mp_VideoErrorIcon);
  videoItem->setData(0, VideoId, id);
  if (error)
    videoItem->setForeground(0, Qt::red);
  return videoItem;
};

QPixmap ModelDockWidget::createSegmentIcon(const QColor& c) const
{
  QColor col = c;
  QPen p = Qt::NoPen;
  QImage segmentImage(16, 16, QImage::Format_ARGB32);
  QPainter painter(&segmentImage);
  painter.setRenderHint(QPainter::Antialiasing);
  if (((c.red() >= 240) && (c.green() >= 240) && (c.blue() >= 240))
      && ((c.red() == c.green()) && (c.red() == c.blue())))
    col = QColor(240,240,240); // very light gray
  p = col;
  painter.setBrush(Qt::NoBrush);
  painter.setPen(Qt::NoPen);
  painter.setCompositionMode(QPainter::CompositionMode_Source);
  painter.fillRect(segmentImage.rect(), Qt::transparent);
  painter.setCompositionMode(QPainter::CompositionMode_SourceOver);
  p.setWidthF(1.5); painter.setPen(p);
  painter.drawEllipse(QRectF(0.75, 9.75, 5.5, 5.5));
  painter.drawEllipse(QRectF(9.75, 0.75, 5.5, 5.5));
  p.setWidthF(2.0); painter.setPen(p);
  painter.drawLine(QLineF(6.0, 10.0, 10.0, 6.0));
  
  return QPixmap::fromImage(segmentImage);
};

QPixmap ModelDockWidget::createMarkerIcon(const QColor& c, bool circled, bool enabled) const
{
  QColor col = c;
  QPen p = Qt::NoPen;
  QImage markerImage(16, 16, QImage::Format_ARGB32);
  QPainter painter(&markerImage);
  painter.setRenderHint(QPainter::Antialiasing);
  if (((c.red() >= 240) && (c.green() >= 240) && (c.blue() >= 240))
      && ((c.red() == c.green()) && (c.red() == c.blue())))
  {
    col = QColor(240,240,240); // very light gray
    p = col;
  }
  painter.setBrush(Qt::NoBrush);
  painter.setPen(Qt::NoPen);
  painter.setCompositionMode(QPainter::CompositionMode_Source);
  painter.fillRect(markerImage.rect(), Qt::transparent);
  painter.setCompositionMode(QPainter::CompositionMode_SourceOver);
  if (enabled && col.isValid())
  {
    QRadialGradient gradient = QRadialGradient(6.0, 6.0, 8.0);
    gradient.setColorAt(0, Qt::white);
    gradient.setColorAt(1, col);
    painter.setBrush(gradient);
    painter.setPen(p);
  }
  if (!enabled || circled)
    painter.setPen(Qt::lightGray);
  painter.drawEllipse(QRectF(1.5, 1.5, 13.0, 13.0));
  return QPixmap::fromImage(markerImage);
};

void ModelDockWidget::editMarkersColor(const QColor& color)
{
  QList<QTreeWidgetItem*> items = this->modelTree->selectedItems();
  // Upate the tree
  QVector<int> ids = QVector<int>(items.count());
  int inc = 0;
  if (this->propertiesStack->currentWidget() == this->segmentPage)
  {
    for (QList<QTreeWidgetItem*>::const_iterator it = items.begin() ; it != items.end() ; ++it)
      ids[inc++] = (*it)->data(0, SegmentId).toInt();
    emit segmentsColorChanged(ids, color);
  }
  else if (this->propertiesStack->currentWidget() == this->markerPage)
  {
    for (QList<QTreeWidgetItem*>::const_iterator it = items.begin() ; it != items.end() ; ++it)
      ids[inc++] = (*it)->data(0, PointId).toInt();
    emit markersColorChanged(ids, color);
  }
  else
  {
    qDebug("Unknown current page during the color edition.");
    return;
  }
  // Upate the button
  this->markerColorButton->setProperty("markerColor", color);
  this->markerColorButton->setIcon(this->createMarkerIcon(color, true));
  // Update the recents colors
  this->pushRecentColor(color);
};

void ModelDockWidget::setMarkersColor(const QVector<int>& ids, const QVector<QColor>& colors)
{
  this->modelTree->blockSignals(true);
  QList<QTreeWidgetItem*> roots;
  roots << this->modelTree->topLevelItem(MarkersItem);
  roots << this->modelTree->topLevelItem(VirtualMarkersItem);
  for (int i = 0 ; i < ids.size() ; ++i)
  {
    for (QList<QTreeWidgetItem*>::iterator itR = roots.begin() ; itR != roots.end() ; ++itR)
    {
      bool found = false;
      for (int j = 0 ; j < (*itR)->childCount() ; ++j)
      {
        QTreeWidgetItem* item = (*itR)->child(j);
        if (item->data(0, PointId).toInt() == ids[i])
        {
          item->setIcon(0, this->createMarkerIcon(colors[i]));
          found = true;
          break;
        }
      }
      if (found) break;
    }
  }
  this->modelTree->blockSignals(false);
  this->displayProperties();
};

void ModelDockWidget::setRecentColor(int idx, const QColor& color)
{
  if (this->m_RecentColors[idx] == color)
    return;
  for (int i = 0 ; i < (this->m_RecentColors.size()-1) ; ++i)
  {
    if (this->m_RecentColors[i] == color)
    {
      this->m_RecentColors[i] = QColor();
      break;
    }
  }
  this->m_RecentColors[idx] = color;
  this->drawRecentColors();
};

void ModelDockWidget::pushRecentColor(const QColor& color)
{
  QColor c = color; // Requires a copy when removing and assigning the same color ...
  int invalidIndex = -1;
  for (int i = 0 ; i < (this->m_RecentColors.size()-1) ; ++i)
  {
    if (!this->m_RecentColors[i].isValid() && this->m_RecentColors[i+1].isValid())
    {
      invalidIndex = i;
      break;
    }
  }
  if (invalidIndex == -1)
  {
    for (int i = 0 ; i < this->m_RecentColors.size() ; ++i)
    {
      if (this->m_RecentColors[i] == color)
      {
        this->m_RecentColors.remove(i);
        break;
      }
    }
    this->m_RecentColors.push_front(c);
    this->m_RecentColors.resize(5);
  }
  else
  {
    for (int i = invalidIndex ; i > 0 ; --i)
      this->m_RecentColors[i] = this->m_RecentColors[i-1];
    this->setRecentColor(0, c);
  }
  this->drawRecentColors();
};

void ModelDockWidget::drawRecentColors()
{
  this->markerColorRecent1->setIcon(this->createMarkerIcon(this->m_RecentColors[0],true));
  this->markerColorRecent2->setIcon(this->createMarkerIcon(this->m_RecentColors[1],true));
  this->markerColorRecent3->setIcon(this->createMarkerIcon(this->m_RecentColors[2],true));
  this->markerColorRecent4->setIcon(this->createMarkerIcon(this->m_RecentColors[3],true));
  this->markerColorRecent5->setIcon(this->createMarkerIcon(this->m_RecentColors[4],true));
};

void ModelDockWidget::refresh()
{
  this->mp_SelectAllMarkers->setEnabled(false);
  this->mp_SelectAllAnalogs->setEnabled(false);
  this->mp_SelectAllModelOutputs->setEnabled(false);
  this->mp_SelectAllAngles->setEnabled(false);
  this->mp_SelectAllForces->setEnabled(false);
  this->mp_SelectAllMoments->setEnabled(false);
  this->mp_SelectAllPowers->setEnabled(false);
  this->mp_SelectAllScalars->setEnabled(false);
    
  QTreeWidgetItem* segmentsRoot = this->modelTree->topLevelItem(SegmentsItem);
  QTreeWidgetItem* markersRoot = this->modelTree->topLevelItem(MarkersItem);
  QTreeWidgetItem* virtualMarkersRoot = this->modelTree->topLevelItem(VirtualMarkersItem);
  QTreeWidgetItem* analogsRoot = this->modelTree->topLevelItem(AnalogsItem);
  QTreeWidgetItem* forcePlatesRoot = this->modelTree->topLevelItem(ForcePlatesItem);
  QTreeWidgetItem* modelOutputsRoot = this->modelTree->topLevelItem(ModelOutputsItem);
  QTreeWidgetItem* modelOutputAngles = modelOutputsRoot->child(0);
  QTreeWidgetItem* modelOutputForces = modelOutputsRoot->child(1);
  QTreeWidgetItem* modelOutputMoments = modelOutputsRoot->child(2);
  QTreeWidgetItem* modelOutputPowers = modelOutputsRoot->child(3);
  QTreeWidgetItem* modelOutputScalars = modelOutputsRoot->child(4);
  QTreeWidgetItem* videosRoot = this->modelTree->topLevelItem(VideosItem);
  
  segmentsRoot->setHidden(true);
  markersRoot->setHidden(true);
  virtualMarkersRoot->setHidden(true);
  analogsRoot->setHidden(true);
  forcePlatesRoot->setHidden(true);
  modelOutputsRoot->setHidden(true);
  modelOutputAngles->setHidden(true);
  modelOutputForces->setHidden(true);
  modelOutputMoments->setHidden(true);
  modelOutputPowers->setHidden(true);
  modelOutputScalars->setHidden(true);
  videosRoot->setHidden(true);
  
  if ((segmentsRoot->childCount() != 0) && this->hasChildVisible(segmentsRoot))
  {
    segmentsRoot->setHidden(false);
  }
  if ((markersRoot->childCount() != 0) && this->hasChildVisible(markersRoot))
  {
    this->mp_SelectAllMarkers->setEnabled(true);
    markersRoot->setHidden(false);
  }
  if ((virtualMarkersRoot->childCount() != 0) && this->hasChildVisible(virtualMarkersRoot))
  {
    this->mp_SelectAllMarkers->setEnabled(true);
    virtualMarkersRoot->setHidden(false);
  }
  if ((analogsRoot->childCount() != 0) && this->hasChildVisible(analogsRoot))
  {
    this->mp_SelectAllAnalogs->setEnabled(true);
    analogsRoot->setHidden(false);
  }
  if ((forcePlatesRoot->childCount() != 0) && this->hasChildVisible(forcePlatesRoot))
  {
    forcePlatesRoot->setHidden(false);
  }
  if ((modelOutputAngles->childCount() != 0) && this->hasChildVisible(modelOutputAngles))
  {
    this->mp_SelectAllAngles->setEnabled(true);
    this->mp_SelectAllModelOutputs->setEnabled(true);
    modelOutputAngles->setHidden(false);
    modelOutputsRoot->setHidden(false);
  }
  if ((modelOutputForces->childCount() != 0) && this->hasChildVisible(modelOutputForces))
  {
    this->mp_SelectAllForces->setEnabled(true);
    this->mp_SelectAllModelOutputs->setEnabled(true);
    modelOutputForces->setHidden(false);
    modelOutputsRoot->setHidden(false);
  }
  if ((modelOutputMoments->childCount() != 0) && this->hasChildVisible(modelOutputMoments))
  {
    this->mp_SelectAllMoments->setEnabled(true);
    this->mp_SelectAllModelOutputs->setEnabled(true);
    modelOutputMoments->setHidden(false);
    modelOutputsRoot->setHidden(false);
  }
  if ((modelOutputPowers->childCount() != 0) && this->hasChildVisible(modelOutputPowers))
  {
    this->mp_SelectAllPowers->setEnabled(true);
    this->mp_SelectAllModelOutputs->setEnabled(true);
    modelOutputPowers->setHidden(false);
    modelOutputsRoot->setHidden(false);
  }
  if ((modelOutputScalars->childCount() != 0) && this->hasChildVisible(modelOutputScalars))
  {
    this->mp_SelectAllScalars->setEnabled(true);
    this->mp_SelectAllModelOutputs->setEnabled(true);
    modelOutputScalars->setHidden(false);
    modelOutputsRoot->setHidden(false);
  }
  if ((videosRoot->childCount() != 0) && this->hasChildVisible(videosRoot))
  {
    videosRoot->setHidden(false);
  }
};

QTreeWidgetItem* ModelDockWidget::treePointChild(QTreeWidgetItem* parent, int id) const
{
  for (int i = 0 ; i < parent->childCount() ; ++i)
  {
    QTreeWidgetItem* child = parent->child(i);
    if (child->data(0, PointId).toInt() == id)
      return child;
  }
  return 0;
};

bool ModelDockWidget::hasChildVisible(QTreeWidgetItem* parent) const
{
  for (int i = 0 ; i < parent->childCount() ; ++i)
  {
    if (!parent->child(i)->isHidden())
      return true;
  }
  return false;
};

bool ModelDockWidget::isOkToModifyAnalog()
{
  QMessageBox messageBox(QMessageBox::Information, 
                         "",
                         tr("The modification of this parameter might corrupt the data when saved in a file."),
                         QMessageBox::Yes | QMessageBox::No,
                         this->parentWidget());
  messageBox.setInformativeText("Do you want to continue?");
#ifdef Q_OS_MAC
  messageBox.setWindowModality(Qt::WindowModal);
  messageBox.setWindowFlags(Qt::Sheet);
#endif
  messageBox.setDefaultButton(QMessageBox::Yes);
  messageBox.setEscapeButton(QMessageBox::No);
  if (messageBox.exec() == QMessageBox::Yes)
    return true;
  return false;
};