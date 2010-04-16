namespace WCFNanoGui
{
    partial class MainForm
    {
        /// <summary>
        /// Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Clean up any resources being used.
        /// </summary>
        /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows Form Designer generated code

        /// <summary>
        /// Required method for Designer support - do not modify
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            this.imageOutput = new System.Windows.Forms.PictureBox();
            this.panel1 = new System.Windows.Forms.Panel();
            this.lbStatus = new System.Windows.Forms.Label();
            this.progressBar1 = new System.Windows.Forms.ProgressBar();
            this.btnCalculate = new System.Windows.Forms.Button();
            this.comboBox1 = new System.Windows.Forms.ComboBox();
            this.label1 = new System.Windows.Forms.Label();
            this.lbInputFile = new System.Windows.Forms.Label();
            this.trackBar1 = new System.Windows.Forms.TrackBar();
            this.trackBar2 = new System.Windows.Forms.TrackBar();
            this.trackBar3 = new System.Windows.Forms.TrackBar();
            this.btnVisualize = new System.Windows.Forms.Button();
            this.label2 = new System.Windows.Forms.Label();
            this.panel2 = new System.Windows.Forms.Panel();
            this.backgroundWorkerVisualize = new System.ComponentModel.BackgroundWorker();
            this.backgroundWorkerCalculate = new System.ComponentModel.BackgroundWorker();
            ((System.ComponentModel.ISupportInitialize)(this.imageOutput)).BeginInit();
            this.panel1.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.trackBar1)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.trackBar2)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.trackBar3)).BeginInit();
            this.panel2.SuspendLayout();
            this.SuspendLayout();
            // 
            // imageOutput
            // 
            this.imageOutput.BackColor = System.Drawing.Color.White;
            this.imageOutput.Location = new System.Drawing.Point(66, 30);
            this.imageOutput.Name = "imageOutput";
            this.imageOutput.Size = new System.Drawing.Size(800, 600);
            this.imageOutput.TabIndex = 0;
            this.imageOutput.TabStop = false;
            // 
            // panel1
            // 
            this.panel1.Controls.Add(this.lbStatus);
            this.panel1.Controls.Add(this.progressBar1);
            this.panel1.Controls.Add(this.btnCalculate);
            this.panel1.Controls.Add(this.comboBox1);
            this.panel1.Controls.Add(this.label1);
            this.panel1.Controls.Add(this.lbInputFile);
            this.panel1.Dock = System.Windows.Forms.DockStyle.Top;
            this.panel1.Location = new System.Drawing.Point(0, 0);
            this.panel1.Name = "panel1";
            this.panel1.Size = new System.Drawing.Size(924, 82);
            this.panel1.TabIndex = 1;
            // 
            // lbStatus
            // 
            this.lbStatus.Location = new System.Drawing.Point(408, 9);
            this.lbStatus.Name = "lbStatus";
            this.lbStatus.Size = new System.Drawing.Size(503, 28);
            this.lbStatus.TabIndex = 7;
            // 
            // progressBar1
            // 
            this.progressBar1.Location = new System.Drawing.Point(408, 42);
            this.progressBar1.MarqueeAnimationSpeed = 0;
            this.progressBar1.Name = "progressBar1";
            this.progressBar1.Size = new System.Drawing.Size(503, 23);
            this.progressBar1.Style = System.Windows.Forms.ProgressBarStyle.Marquee;
            this.progressBar1.TabIndex = 6;
            // 
            // btnCalculate
            // 
            this.btnCalculate.Location = new System.Drawing.Point(327, 42);
            this.btnCalculate.Name = "btnCalculate";
            this.btnCalculate.Size = new System.Drawing.Size(75, 23);
            this.btnCalculate.TabIndex = 2;
            this.btnCalculate.Text = "Запуск";
            this.btnCalculate.UseVisualStyleBackColor = true;
            this.btnCalculate.Click += new System.EventHandler(this.btnCalculate_Click);
            // 
            // comboBox1
            // 
            this.comboBox1.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.comboBox1.FormattingEnabled = true;
            this.comboBox1.Location = new System.Drawing.Point(96, 42);
            this.comboBox1.Name = "comboBox1";
            this.comboBox1.Size = new System.Drawing.Size(225, 21);
            this.comboBox1.TabIndex = 1;
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Font = new System.Drawing.Font("Microsoft Sans Serif", 12F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(204)));
            this.label1.Location = new System.Drawing.Point(11, 7);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(113, 20);
            this.label1.TabIndex = 0;
            this.label1.Text = "Расчет ORCA";
            // 
            // lbInputFile
            // 
            this.lbInputFile.AutoSize = true;
            this.lbInputFile.Location = new System.Drawing.Point(12, 47);
            this.lbInputFile.Name = "lbInputFile";
            this.lbInputFile.Size = new System.Drawing.Size(78, 13);
            this.lbInputFile.TabIndex = 0;
            this.lbInputFile.Text = "Входной файл";
            // 
            // trackBar1
            // 
            this.trackBar1.Location = new System.Drawing.Point(273, 636);
            this.trackBar1.Maximum = 360;
            this.trackBar1.Name = "trackBar1";
            this.trackBar1.Size = new System.Drawing.Size(364, 45);
            this.trackBar1.TabIndex = 2;
            this.trackBar1.TickFrequency = 10;
            this.trackBar1.TickStyle = System.Windows.Forms.TickStyle.Both;
            this.trackBar1.Value = 180;
            // 
            // trackBar2
            // 
            this.trackBar2.Location = new System.Drawing.Point(16, 141);
            this.trackBar2.Maximum = 360;
            this.trackBar2.Name = "trackBar2";
            this.trackBar2.Orientation = System.Windows.Forms.Orientation.Vertical;
            this.trackBar2.Size = new System.Drawing.Size(45, 286);
            this.trackBar2.TabIndex = 2;
            this.trackBar2.TickFrequency = 10;
            this.trackBar2.TickStyle = System.Windows.Forms.TickStyle.Both;
            this.trackBar2.Value = 180;
            // 
            // trackBar3
            // 
            this.trackBar3.Location = new System.Drawing.Point(872, 141);
            this.trackBar3.Maximum = 360;
            this.trackBar3.Name = "trackBar3";
            this.trackBar3.Orientation = System.Windows.Forms.Orientation.Vertical;
            this.trackBar3.Size = new System.Drawing.Size(45, 286);
            this.trackBar3.TabIndex = 2;
            this.trackBar3.TickFrequency = 10;
            this.trackBar3.TickStyle = System.Windows.Forms.TickStyle.Both;
            this.trackBar3.Value = 180;
            // 
            // btnVisualize
            // 
            this.btnVisualize.Location = new System.Drawing.Point(821, 648);
            this.btnVisualize.Name = "btnVisualize";
            this.btnVisualize.Size = new System.Drawing.Size(91, 23);
            this.btnVisualize.TabIndex = 3;
            this.btnVisualize.Text = "Визуализация";
            this.btnVisualize.UseVisualStyleBackColor = true;
            this.btnVisualize.Click += new System.EventHandler(this.btnVisualize_Click);
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Font = new System.Drawing.Font("Microsoft Sans Serif", 12F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(204)));
            this.label2.Location = new System.Drawing.Point(12, 3);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(116, 20);
            this.label2.TabIndex = 0;
            this.label2.Text = "Визуализация";
            // 
            // panel2
            // 
            this.panel2.Controls.Add(this.imageOutput);
            this.panel2.Controls.Add(this.trackBar1);
            this.panel2.Controls.Add(this.trackBar2);
            this.panel2.Controls.Add(this.label2);
            this.panel2.Controls.Add(this.trackBar3);
            this.panel2.Controls.Add(this.btnVisualize);
            this.panel2.Dock = System.Windows.Forms.DockStyle.Fill;
            this.panel2.Enabled = false;
            this.panel2.Location = new System.Drawing.Point(0, 82);
            this.panel2.Name = "panel2";
            this.panel2.Size = new System.Drawing.Size(924, 683);
            this.panel2.TabIndex = 4;
            // 
            // backgroundWorkerVisualize
            // 
            this.backgroundWorkerVisualize.WorkerReportsProgress = true;
            this.backgroundWorkerVisualize.DoWork += new System.ComponentModel.DoWorkEventHandler(this.backgroundWorkerVisualize_DoWork);
            this.backgroundWorkerVisualize.RunWorkerCompleted += new System.ComponentModel.RunWorkerCompletedEventHandler(this.backgroundWorkerVisualize_RunWorkerCompleted);
            this.backgroundWorkerVisualize.ProgressChanged += new System.ComponentModel.ProgressChangedEventHandler(this.backgroundWorkerVisualize_ProgressChanged);
            // 
            // backgroundWorkerCalculate
            // 
            this.backgroundWorkerCalculate.DoWork += new System.ComponentModel.DoWorkEventHandler(this.backgroundWorkerCalculate_DoWork);
            this.backgroundWorkerCalculate.RunWorkerCompleted += new System.ComponentModel.RunWorkerCompletedEventHandler(this.backgroundWorkerCalculate_RunWorkerCompleted);
            // 
            // MainForm
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(924, 765);
            this.Controls.Add(this.panel2);
            this.Controls.Add(this.panel1);
            this.Name = "MainForm";
            this.Text = "NanoGui";
            this.Load += new System.EventHandler(this.MainForm_Load);
            ((System.ComponentModel.ISupportInitialize)(this.imageOutput)).EndInit();
            this.panel1.ResumeLayout(false);
            this.panel1.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.trackBar1)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.trackBar2)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.trackBar3)).EndInit();
            this.panel2.ResumeLayout(false);
            this.panel2.PerformLayout();
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.PictureBox imageOutput;
        private System.Windows.Forms.Panel panel1;
        private System.Windows.Forms.Label lbInputFile;
        private System.Windows.Forms.TrackBar trackBar1;
        private System.Windows.Forms.TrackBar trackBar2;
        private System.Windows.Forms.TrackBar trackBar3;
        private System.Windows.Forms.Button btnCalculate;
        private System.Windows.Forms.ComboBox comboBox1;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.Button btnVisualize;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.Panel panel2;
        private System.Windows.Forms.ProgressBar progressBar1;
        private System.Windows.Forms.Label lbStatus;
        private System.ComponentModel.BackgroundWorker backgroundWorkerVisualize;
        private System.ComponentModel.BackgroundWorker backgroundWorkerCalculate;
    }
}

