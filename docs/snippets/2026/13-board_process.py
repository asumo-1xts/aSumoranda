import sys

pcbnew_path = "/usr/lib/python3/dist-packages"
if pcbnew_path not in sys.path:
    sys.path.insert(0, pcbnew_path)
try:
    import pcbnew
except ImportError as e:
    print(f"Error: Could not import pcbnew. {e}")


class BoardProcessor:
    def __init__(self, board_file, out_dir, target_layer_name):
        self.board = pcbnew.LoadBoard(board_file)
        self.out_dir = out_dir
        self.pctl = pcbnew.PLOT_CONTROLLER(self.board)
        self.target_layer = pcbnew.B_Cu if target_layer_name == "B_Cu" else pcbnew.F_Cu
        self._setup_plot_options()

    def _setup_plot_options(self):
        popt = self.pctl.GetPlotOptions()
        popt.SetOutputDirectory(self.out_dir)
        popt.SetPlotFrameRef(False)
        popt.SetSketchPadLineWidth(pcbnew.FromMM(0.1))
        popt.SetAutoScale(False)
        popt.SetScale(1)
        popt.SetMirror(False)

    def plot_current_state(self, prefix, index):
        filename = f"{prefix}_{index:05d}"
        layers = pcbnew.LSEQ()
        layers.append(self.target_layer)
        layers.append(pcbnew.Edge_Cuts)

        self.pctl.OpenPlotfile(filename, pcbnew.PLOT_FORMAT_SVG, "Step Plot")
        self.pctl.PlotLayers(layers)
        self.pctl.ClosePlot()

    def get_all_items(self):
        return {
            "ZONE": list(self.board.Zones()),
            "TRACK": list(self.board.GetTracks()),
            "FOOTPRINT": list(self.board.GetFootprints()),
            "DRAWING": list(self.board.GetDrawings()),
        }

    def remove_item(self, item):
        """物理削除。Cレベルの警告出力を完全に封じ込める"""
        try:
            self.board.Remove(item)
        except Exception:
            pass
