import os

from OCC.Core.TopoDS import TopoDS_Shape
from OCC.Core.TopAbs import TopAbs_SOLID, TopAbs_SHELL, TopAbs_COMPOUND
from OCC.Core.BRepTools import breptools
from OCC.Core.BRepMesh import BRepMesh_IncrementalMesh
from OCC.Core.StlAPI import stlapi, StlAPI_Writer
from OCC.Core.BRep import BRep_Builder
from OCC.Core.gp import gp_Pnt, gp_Dir, gp_Pnt2d
from OCC.Core.Bnd import Bnd_Box2d
from OCC.Core.TopoDS import TopoDS_Compound
from OCC.Core.IGESControl import (
    IGESControl_Controller,
    IGESControl_Reader,
    IGESControl_Writer,
)
from OCC.Core.STEPControl import (
    STEPControl_Reader,
    STEPControl_Writer,
    STEPControl_AsIs,
)
from OCC.Core.Interface import Interface_Static
from OCC.Core.IFSelect import IFSelect_RetDone, IFSelect_ItemsByEntity
from OCC.Core.TDocStd import TDocStd_Document
from OCC.Core.XCAFDoc import (
    XCAFDoc_DocumentTool,
    XCAFDoc_ColorTool,
)
from OCC.Core.STEPCAFControl import STEPCAFControl_Reader
from OCC.Core.TDF import TDF_LabelSequence, TDF_Label
from OCC.Core.Quantity import Quantity_Color, Quantity_TOC_RGB
from OCC.Core.TopLoc import TopLoc_Location
from OCC.Core.BRepBuilderAPI import BRepBuilderAPI_Transform
from OCC.Core.TColStd import TColStd_IndexedDataMapOfStringString
from OCC.Core.TCollection import TCollection_AsciiString
from OCC.Core.RWPly import RWPly_CafWriter
from OCC.Core.Message import Message_ProgressRange

from OCC.Core.RWGltf import RWGltf_CafReader, RWGltf_CafWriter
from OCC.Core.RWObj import RWObj_CafWriter, RWObj_CafReader
from OCC.Core.RWMesh import (
    RWMesh_CoordinateSystem_posYfwd_posZup,
    RWMesh_CoordinateSystem_negZfwd_posYup,
)
from OCC.Core.UnitsMethods import unitsmethods


def read_step_file_with_names_colors(filename):
    """Returns list of tuples (topods_shape, label, color)
    Use OCAF.
    """
    if not os.path.isfile(filename):
        raise FileNotFoundError(f"{filename} not found.")
    # the list:
    output_shapes = {}

    # create an handle to a document
    doc = TDocStd_Document("pythonocc-doc-step-import")

    # Get root assembly
    shape_tool = XCAFDoc_DocumentTool.ShapeTool(doc.Main())
    color_tool = XCAFDoc_DocumentTool.ColorTool(doc.Main())
    # layer_tool = XCAFDoc_DocumentTool_LayerTool(doc.Main())
    # mat_tool = XCAFDoc_DocumentTool_MaterialTool(doc.Main())

    step_reader = STEPCAFControl_Reader()
    step_reader.SetColorMode(True)
    step_reader.SetLayerMode(True)
    step_reader.SetNameMode(True)
    step_reader.SetMatMode(True)
    step_reader.SetGDTMode(True)

    status = step_reader.ReadFile(filename)
    if status == IFSelect_RetDone:
        step_reader.Transfer(doc)

    locs = []

    def _get_sub_shapes(lab=TDF_Label(), loc=TopLoc_Location(), depth=0, prefix="0:"):
        # global cnt, lvl
        # cnt += 1
        # print("\n[%d] level %d, handling LABEL %s\n" % (cnt, lvl, _get_label_name(lab)))
        # print()
        # print(lab.DumpToString())
        # print()
        # print("Is Assembly    :", shape_tool.IsAssembly(lab))
        # print("Is Free        :", shape_tool.IsFree(lab))
        # print("Is Shape       :", shape_tool.IsShape(lab))
        # print("Is Compound    :", shape_tool.IsCompound(lab))
        # print("Is Component   :", shape_tool.IsComponent(lab))
        # print("Is SimpleShape :", shape_tool.IsSimpleShape(lab))
        # print("Is Reference   :", shape_tool.IsReference(lab))

        # users = TDF_LabelSequence()
        # users_cnt = shape_tool.GetUsers(lab, users)
        # print("Nr Users       :", users_cnt)

        l_subss = TDF_LabelSequence()
        shape_tool.GetSubShapes(lab, l_subss)
        # print("Nb subshapes   :", l_subss.Length())
        l_comps = TDF_LabelSequence()
        shape_tool.GetComponents(lab, l_comps)
        # print("Nb components  :", l_comps.Length())
        # print()
            
        name = lab.GetLabelName()
        print(f"Name & Label: {lab.EntryDump()} {name}")

        if shape_tool.IsAssembly(lab):
            l_c = TDF_LabelSequence()
            shape_tool.GetComponents(lab, l_c)
            for i in range(l_c.Length()):
                label = l_c.Value(i + 1)
                if shape_tool.IsReference(label):
                    # print("\n########  reference label :", label)
                    label_reference = TDF_Label()
                    shape_tool.GetReferredShape(label, label_reference)
                    loc = shape_tool.GetLocation(label)
                    # print("    loc          :", loc)
                    # trans = loc.Transformation()
                    # print("    tran form    :", trans.Form())
                    # rot = trans.GetRotation()
                    # print("    rotation     :", rot)
                    # print("    X            :", rot.X())
                    # print("    Y            :", rot.Y())
                    # print("    Z            :", rot.Z())
                    # print("    W            :", rot.W())
                    # tran = trans.TranslationPart()
                    # print("    translation  :", tran)
                    # print("    X            :", tran.X())
                    # print("    Y            :", tran.Y())
                    # print("    Z            :", tran.Z())

                    locs.append(loc)
                    # print(">>>>")
                    # lvl += 1
                    _get_sub_shapes(label_reference, loc)
                    # lvl -= 1
                    # print("<<<<")
                    locs.pop()

        elif shape_tool.IsSimpleShape(lab):
            # print("\n########  simpleshape label :", lab)
            shape = shape_tool.GetShape(lab)
            # print("    all ass locs   :", locs)

            loc = TopLoc_Location()
            for l in locs:
                # print("    take loc       :", l)
                loc = loc.Multiplied(l)

            # trans = loc.Transformation()
            # print("    FINAL loc    :")
            # print("    tran form    :", trans.Form())
            # rot = trans.GetRotation()
            # print("    rotation     :", rot)
            # print("    X            :", rot.X())
            # print("    Y            :", rot.Y())
            # print("    Z            :", rot.Z())
            # print("    W            :", rot.W())
            # tran = trans.TranslationPart()
            # print("    translation  :", tran)
            # print("    X            :", tran.X())
            # print("    Y            :", tran.Y())
            # print("    Z            :", tran.Z())
            c = Quantity_Color(0.5, 0.5, 0.5, Quantity_TOC_RGB)  # default color
            color_set = False
            if (
                color_tool.GetInstanceColor(shape, 0, c)
                or color_tool.GetInstanceColor(shape, 1, c)
                or color_tool.GetInstanceColor(shape, 2, c)
            ):
                color_tool.SetInstanceColor(shape, 0, c)
                color_tool.SetInstanceColor(shape, 1, c)
                color_tool.SetInstanceColor(shape, 2, c)
                color_set = True
                n = c.Name(c.Red(), c.Green(), c.Blue())
                print(
                    "instance color Name & RGB: ",
                    c,
                    n,
                    c.Red(),
                    c.Green(),
                    c.Blue(),
                )

            if not color_set:
                if (
                    XCAFDoc_ColorTool.GetColor(lab, 0, c)
                    or XCAFDoc_ColorTool.GetColor(lab, 1, c)
                    or XCAFDoc_ColorTool.GetColor(lab, 2, c)
                ):
                    color_tool.SetInstanceColor(shape, 0, c)
                    color_tool.SetInstanceColor(shape, 1, c)
                    color_tool.SetInstanceColor(shape, 2, c)

                    n = c.Name(c.Red(), c.Green(), c.Blue())
                    print(
                        "shape color Name & RGB: ",
                        c,
                        n,
                        c.Red(),
                        c.Green(),
                        c.Blue(),
                    )

            shape_disp = BRepBuilderAPI_Transform(shape, loc.Transformation()).Shape()
            
            trans = loc.Transformation()
            print("Subshape Transformation:")
            print("  Form        :", trans.Form())
            rot = trans.GetRotation()
            print("  Rotation    :", rot)
            print("    X         :", rot.X())
            print("    Y         :", rot.Y())
            print("    Z         :", rot.Z())
            print("    W         :", rot.W())
            tran = trans.TranslationPart()
            print("  Translation :", tran)
            print("    X         :", tran.X())
            print("    Y         :", tran.Y())
            print("    Z         :", tran.Z())
            if shape_disp not in output_shapes:
                output_shapes[shape_disp] = [lab.GetLabelName(), c]
            for i in range(l_subss.Length()):
                lab_subs = l_subss.Value(i + 1)
                # print("\n########  simpleshape subshape label :", lab)
                shape_sub = shape_tool.GetShape(lab_subs)

                c = Quantity_Color(0.5, 0.5, 0.5, Quantity_TOC_RGB)  # default color
                color_set = False
                if (
                    color_tool.GetInstanceColor(shape_sub, 0, c)
                    or color_tool.GetInstanceColor(shape_sub, 1, c)
                    or color_tool.GetInstanceColor(shape_sub, 2, c)
                ):
                    color_tool.SetInstanceColor(shape_sub, 0, c)
                    color_tool.SetInstanceColor(shape_sub, 1, c)
                    color_tool.SetInstanceColor(shape_sub, 2, c)
                    color_set = True
                    n = c.Name(c.Red(), c.Green(), c.Blue())
                    print(
                        "    instance color Name & RGB: ",
                        c,
                        n,
                        c.Red(),
                        c.Green(),
                        c.Blue(),
                    )

                if not color_set:
                    if (
                        XCAFDoc_ColorTool.GetColor(lab_subs, 0, c)
                        or XCAFDoc_ColorTool.GetColor(lab_subs, 1, c)
                        or XCAFDoc_ColorTool.GetColor(lab_subs, 2, c)
                    ):
                        color_tool.SetInstanceColor(shape, 0, c)
                        color_tool.SetInstanceColor(shape, 1, c)
                        color_tool.SetInstanceColor(shape, 2, c)

                        n = c.Name(c.Red(), c.Green(), c.Blue())
                        print(
                            "    shape color Name & RGB: ",
                            c,
                            n,
                            c.Red(),
                            c.Green(),
                            c.Blue(),
                        )
                shape_to_disp = BRepBuilderAPI_Transform(
                    shape_sub, loc.Transformation()
                ).Shape()
                # position the subshape to display
                
                if shape_to_disp not in output_shapes:
                    output_shapes[shape_to_disp] = [lab_subs.GetLabelName(), c]

    def _get_shapes():
        labels = TDF_LabelSequence()
        shape_tool.GetShapes(labels)
        # global cnt
        # cnt += 1

        print()
        print("Number of shapes at root :", labels.Length())
        print()
        for i in range(labels.Length()):
            root_item = labels.Value(i + 1)
            _get_sub_shapes(root_item, None)

    _get_shapes()
    return output_shapes


read_step_file_with_names_colors("./QtProject/as1-oc-214.stp")
