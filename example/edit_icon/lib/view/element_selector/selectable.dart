import 'dart:math';

import 'package:flutter/material.dart';

class Selectable extends StatefulWidget {
  const Selectable(
      {Key? key,
      this.isSelected = false,
      required this.dimension,
      this.gap = 8.0,
      this.axis = Axis.vertical,
      this.onTap,
      this.child,
      this.onRemove,
      this.timeToRemove = const Duration(milliseconds: 200)})
      : super(key: key);

  final bool isSelected;
  final Axis axis;
  final Widget? child;
  final double gap;
  final double dimension;
  final void Function()? onTap;
  final void Function()? onRemove;
  final Duration timeToRemove;

  @override
  State<Selectable> createState() => _SelectableState();
}

class _SelectableState extends State<Selectable> {
  final iconInsetFactor = 0.7;
  late bool _isSelected = widget.isSelected;
  bool _removing = false;

  @override
  void didUpdateWidget(Selectable oldWidget) {
    super.didUpdateWidget(oldWidget);
    _isSelected = widget.isSelected;
  }

  @override
  Widget build(BuildContext context) {
    final theme = Theme.of(context);
    final transparent = theme.colorScheme.background.withOpacity(0.0);

    return Padding(
      padding: EdgeInsets.symmetric(
          vertical: widget.axis == Axis.vertical ? widget.gap : 0,
          horizontal: widget.axis == Axis.horizontal ? widget.gap : 0),
      child: Stack(
        alignment: Alignment.center,
        children: [
          SizedBox.square(
            dimension: widget.dimension,
            child: Material(
              clipBehavior: Clip.antiAlias,
              type: MaterialType.circle,
              color: transparent,
              child: InkWell(
                hoverColor: _removing ? transparent : null,
                highlightColor: _removing ? transparent : null,
                onTap: widget.onTap,
                onFocusChange: (hasFocus) =>
                    hasFocus ? widget.onTap?.call() : null,
                autofocus: true,
                canRequestFocus: true,
                focusColor: transparent,
                child: AnimatedContainer(
                    duration: _removing
                        ? widget.timeToRemove
                        : const Duration(milliseconds: 800),
                    curve: Curves.ease,
                    decoration: BoxDecoration(
                        shape: BoxShape.circle,
                        color: _isSelected
                            ? Color.lerp(theme.colorScheme.background,
                                theme.colorScheme.outline, 0.15)
                            : transparent),
                    foregroundDecoration: BoxDecoration(
                        shape: BoxShape.circle,
                        border: Border.all(
                            width: _isSelected ? 2.0 : 0,
                            color: _isSelected
                                ? theme.colorScheme.outline
                                : transparent)),
                    child: Padding(
                      padding: EdgeInsets.all(sqrt(widget.dimension)),
                      child: AnimatedOpacity(
                          opacity: _removing ? 0.0 : 1.0,
                          duration: widget.timeToRemove,
                          child: widget.child),
                    )),
              ),
            ),
          ),
          if (_isSelected && widget.onRemove != null)
            Positioned.fill(
                left: (widget.dimension -
                        (widget.axis == Axis.horizontal ? widget.gap * 2 : 0)) *
                    iconInsetFactor,
                bottom: (widget.dimension -
                        (widget.axis == Axis.vertical ? widget.gap * 2 : 0)) *
                    iconInsetFactor,
                child: Transform.scale(
                  scale: .6,
                  child: FloatingActionButton(
                    tooltip: "Remove",
                    elevation: 0,
                    focusElevation: 0,
                    highlightElevation: 0,
                    disabledElevation: 0,
                    hoverElevation: 0,
                    onPressed: () async {
                      setState(() {
                        _isSelected = false;
                        _removing = true;
                      });
                      Focus.maybeOf(context)?.unfocus();
                      await Future.delayed(widget.timeToRemove);
                      widget.onRemove!();
                    },
                    child: Transform.rotate(
                        angle: pi / 4,
                        child: const Icon(
                          Icons.add,
                          size: 36,
                        )),
                  ),
                )),
        ],
      ),
    );
  }
}
