class CodeEditor {
    constructor() {
        this.lines = [];
        this.references = {};
    }

    addLine(content) {
        const lineNumber = this.lines.length + 1;
        this.lines.push({ number: lineNumber, content });
        this.updateLineNumbers();
    }

    removeLine(lineNumber) {
        this.lines = this.lines.filter(line => line.number !== lineNumber);
        this.updateLineNumbers();
    }

    updateLineNumbers() {
        this.lines.forEach((line, index) => {
            line.number = index + 1;
        });
    }

    addReference(sourceLine, targetLine) {
        if (!this.references[sourceLine]) {
            this.references[sourceLine] = [];
        }
        this.references[sourceLine].push(targetLine);
    }

    resolveReference(sourceLine) {
        return this.references[sourceLine] || [];
    }
}

// Example Usage
const editor = new CodeEditor();
editor.addLine('let x = 10;');
editor.addLine('console.log(x);');
editor.addReference(2, 1); // Line 2 references Line 1
console.log(editor.resolveReference(2)); // Output: [1]
