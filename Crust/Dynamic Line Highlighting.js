function renderCodeWithLineNumbers(code, containerId) {
    const container = document.getElementById(containerId);
    container.innerHTML = ''; // Clear previous content

    const lines = code.split('\n');
    lines.forEach((line, index) => {
        const lineNumber = document.createElement('span');
        lineNumber.textContent = `${index + 1}`;
        lineNumber.className = 'line-number';

        const lineContent = document.createElement('div');
        lineContent.textContent = line;
        lineContent.className = 'code-line';

        const lineWrapper = document.createElement('div');
        lineWrapper.className = 'line-wrapper';
        lineWrapper.appendChild(lineNumber);
        lineWrapper.appendChild(lineContent);

        container.appendChild(lineWrapper);
    });
}
